// Names the stbc-decomp globals in the open program from build/ghidra_data.csv ("address,name[,decorated]", written
// by tools/ghidra/export_data.py). A C++ global's decorated name goes through Ghidra's Microsoft demangler first,
// which lays down the global's data type (the class structures ApplyStbcTypes.java created, found by name); the
// project's name is then the primary label, "Class::member" in the namespace Class.
// Run it from the Script Manager after ApplyStbcTypes.java, then save the program.
//@category stbc-decomp
import ghidra.app.script.GhidraScript;
import ghidra.app.util.NamespaceUtils;
import ghidra.app.util.demangler.DemangledObject;
import ghidra.app.util.demangler.DemanglerOptions;
import ghidra.app.util.demangler.MangledContext;
import ghidra.app.util.demangler.microsoft.MicrosoftDemangler;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.*;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

public class ApplyStbcData extends GhidraScript {

	private final MicrosoftDemangler demangler = new MicrosoftDemangler();

	@Override
	public void run() throws Exception {
		// ApplyStbcAll.java passes the file as the script argument; run alone, the script asks for it
		File csv = getScriptArgs().length > 0 ? new File(getScriptArgs()[0]) : askFile("stbc-decomp globals (build/ghidra_data.csv)", "Apply");
		DemanglerOptions options = demangler.createDefaultOptions();
		options.setApplySignature(true);
		options.setDoDisassembly(false);
		SymbolTable table = currentProgram.getSymbolTable();
		int named = 0, typed = 0, failed = 0;
		try (BufferedReader reader = new BufferedReader(new FileReader(csv))) {
			String line;
			while ((line = reader.readLine()) != null && !monitor.isCancelled()) {
				line = line.trim();
				if (line.isEmpty() || line.startsWith("#")) {
					continue;
				}
				int first = line.indexOf(',');
				int last = line.lastIndexOf(',');
				Address address = toAddr(line.substring(0, first).trim());
				String name = line.substring(first + 1, last > first ? last : line.length()).trim();
				String decorated = last > first ? line.substring(last + 1).trim() : "";
				try {
					// labels an earlier run left would make the demangler think the address is already done
					for (Symbol s : table.getSymbols(address)) {
						if (s.getSource() == SourceType.USER_DEFINED && s.getSymbolType() == SymbolType.LABEL) {
							s.delete();
						}
					}
					if (decorated.startsWith("?")) {
						MangledContext context = demangler.createMangledContext(decorated, options, currentProgram, address);
						DemangledObject demangled = demangler.demangle(context);
						if (demangled != null && demangled.applyUsingContext(monitor)) {
							typed++;
						}
					}
					Namespace namespace = currentProgram.getGlobalNamespace();
					int scope = name.lastIndexOf("::");
					if (scope > 0) {
						namespace = NamespaceUtils.createNamespaceHierarchy(name.substring(0, scope), null, currentProgram,
							SourceType.USER_DEFINED);
						name = name.substring(scope + 2);
					}
					Symbol label = table.createLabel(address, name, namespace, SourceType.USER_DEFINED);
					label.setPrimary();
					named++;
				}
				catch (Exception e) {
					println("failed at " + address + " (" + name + "): " + e.getMessage());
					failed++;
				}
			}
		}
		println("globals named " + named + ", typed through the demangler " + typed + ", failed " + failed);
	}
}
