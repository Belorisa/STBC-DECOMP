// Applies the stbc-decomp function names and signatures to the open program: reads a CSV of
// "address,name[,decorated]" lines (written by tools/ghidra/export_names.py), creates the function where Ghidra has
// none, applies the signature the decorated C++ name encodes (calling convention, parameter and return types, the
// class of a member function) through Ghidra's Microsoft demangler, then sets the project's name.
// Run it from the Script Manager, then save the program (File > Save).
//@category stbc-decomp
import ghidra.app.script.GhidraScript;
import ghidra.app.util.NamespaceUtils;
import ghidra.app.util.demangler.DemangledObject;
import ghidra.app.util.demangler.DemanglerOptions;
import ghidra.app.util.demangler.MangledContext;
import ghidra.app.util.demangler.microsoft.MicrosoftDemangler;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Namespace;
import ghidra.program.model.symbol.SourceType;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class ApplyStbcNames extends GhidraScript {

	private final MicrosoftDemangler demangler = new MicrosoftDemangler();

	@Override
	public void run() throws Exception {
		// ApplyStbcAll.java passes the file as the script argument; run alone, the script asks for it
		File csv = getScriptArgs().length > 0 ? new File(getScriptArgs()[0]) : askFile("stbc-decomp names (build/ghidra_names.csv)", "Apply");
		DemanglerOptions options = demangler.createDefaultOptions();
		options.setApplySignature(true);
		options.setApplyCallingConvention(true);
		options.setDoDisassembly(false);
		int created = 0, renamed = 0, typed = 0, untyped = 0, failed = 0;
		try (BufferedReader reader = new BufferedReader(new FileReader(csv))) {
			String line;
			while ((line = reader.readLine()) != null && !monitor.isCancelled()) {
				line = line.trim();
				if (line.isEmpty() || line.startsWith("#")) {
					continue;
				}
				// A template name may hold commas; a decorated name never does, so it is the text after the last one.
				int first = line.indexOf(',');
				int last = line.lastIndexOf(',');
				Address address = toAddr(line.substring(0, first).trim());
				String name = line.substring(first + 1).trim();
				String decorated = "";
				if (last > first) {
					String tail = line.substring(last + 1).trim();
					if (tail.isEmpty() || tail.startsWith("?")) {
						name = line.substring(first + 1, last).trim();
						decorated = tail;
					}
				}

				Function function = getFunctionAt(address);
				if (function == null) {
					Function containing = getFunctionContaining(address);
					if (containing != null) {
						// Ghidra's analysis folded this address into another function; leave that for a human.
						println("inside " + containing.getName() + " at " + containing.getEntryPoint() + ": " + address + " " + name);
						failed++;
						continue;
					}
					disassemble(address);
					function = createFunction(address, null);
					if (function == null) {
						println("could not create a function at " + address + " (" + name + ")");
						failed++;
						continue;
					}
					created++;
				}
				// The signature first: applying it may also rename, and the project's name below has the last word.
				if (decorated.startsWith("?")) {
					if (applySignature(function, decorated, address, options)) {
						typed++;
					}
					else {
						untyped++;
					}
				}
				if (name.isEmpty()) {
					continue;   // a claimed operator or template: the demangler's name stands
				}
				try {
					// "Class::Method" becomes the function Method in namespace Class, as Ghidra models C++.
					int scope = name.lastIndexOf("::");
					if (scope > 0) {
						Namespace namespace = NamespaceUtils.createNamespaceHierarchy(name.substring(0, scope), null,
							currentProgram, SourceType.USER_DEFINED);
						function.setParentNamespace(namespace);
						name = name.substring(scope + 2);
					}
					function.setName(name, SourceType.USER_DEFINED);
					renamed++;
				}
				catch (Exception e) {
					println("rename failed at " + address + " (" + name + "): " + e.getMessage());
					failed++;
				}
			}
		}
		println("created " + created + ", renamed " + renamed + ", signatures applied " + typed +
			" (not applied " + untyped + "), failed " + failed);
	}

	private boolean applySignature(Function function, String decorated, Address address, DemanglerOptions options) {
		try {
			MangledContext context = demangler.createMangledContext(decorated, options, currentProgram, address);
			DemangledObject demangled = demangler.demangle(context);
			if (demangled == null) {
				return false;
			}
			// The demangler skips an address that already has a symbol of the demangled name in a namespace (the
			// project's own name from an earlier run) and reports success without applying anything: give the
			// function its decorated name first, as a fresh import would have it.
			function.setParentNamespace(currentProgram.getGlobalNamespace());
			function.setName(decorated, SourceType.IMPORTED);
			if (!demangled.applyUsingContext(monitor)) {
				return false;
			}
			return function.getSignatureSource() != SourceType.DEFAULT;
		}
		catch (Exception e) {
			println("signature not applied at " + address + " (" + decorated + "): " + e.getMessage());
			return false;
		}
	}
}
