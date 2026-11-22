// Brings the open program up to date with the stbc-decomp checkout this script lives in: runs ApplyStbcTypes.java,
// ApplyStbcNames.java and ApplyStbcData.java, in that order, on the checkout's build/ghidra_types.json,
// build/ghidra_names.csv and build/ghidra_data.csv (merge_lane.py writes all three at every merge; by hand:
// tools/ghidra/export_types.py, export_names.py and export_data.py after a build). Save the program afterwards.
//@category stbc-decomp
import ghidra.app.script.GhidraScript;

import java.io.File;

public class ApplyStbcAll extends GhidraScript {

	@Override
	public void run() throws Exception {
		// tools/ghidra/ApplyStbcAll.java -> the checkout's build directory
		File build = new File(getSourceFile().getParentFile().getParentFile().getParentFile().getFile(false), "build");
		String[][] steps = {
			{ "ApplyStbcTypes.java", "ghidra_types.json" },
			{ "ApplyStbcNames.java", "ghidra_names.csv" },
			{ "ApplyStbcData.java", "ghidra_data.csv" },
		};
		for (String[] step : steps) {
			File input = new File(build, step[1]);
			if (!input.exists()) {
				printerr(input + " is missing: run tools/ghidra/export_" + step[1].replaceAll("ghidra_|\\..*", "") +
					".py in the checkout first");
				return;
			}
		}
		for (String[] step : steps) {
			if (monitor.isCancelled()) {
				return;
			}
			println("== " + step[0] + " on " + new File(build, step[1]));
			runScript(step[0], new String[] { new File(build, step[1]).getPath() });
		}
		println("done: save the program (File > Save)");
	}
}
