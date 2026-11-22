"""Ghidra's decompilation of a function, from the GhidraMCP plugin's HTTP server of the running Ghidra (the server the
`ghidra` MCP tools talk to). Evidence for a reconstruction, never source: the project's types, names and signatures
make it read close to the original once tools/ghidra's Apply scripts have run.

    import ghidra_http; text = ghidra_http.decompile(0x404A60)   # None when Ghidra isn't running
"""
import json
import re
import urllib.request

SERVER = "http://127.0.0.1:8080"


def decompile(addr, server=SERVER, timeout=60):
    """The C pseudocode of the function at addr, or None when the server is down or has no function there."""
    try:
        with urllib.request.urlopen("%s/decompile_function?address=0x%X" % (server, addr), timeout=timeout) as r:
            data = json.loads(r.read())
    except Exception:
        return None
    if not data.get("success"):
        return None
    code = (data.get("data") or {}).get("code") or ""
    code = code.replace("\r\n", "\n")
    # Ghidra's notes on its own analysis say nothing about the original
    code = re.sub(r"^/\* WARNING:.*?\*/\n", "", code, flags=re.M)
    return code.strip() or None
