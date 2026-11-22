// Creates the stbc-decomp data types in the open program from build/ghidra_types.json (written by
// tools/ghidra/export_types.py from the compiler's own type records): every class, struct, union and enum the source
// defines, with its bases' members flattened in, and a <Class>_vftable structure of typed function pointers that
// the class's 'vftable' field points to. The empty structures Ghidra's demangler created for the same names (the
// 'this' types of the signatures ApplyStbcNames.java applied) are replaced by these, so every function that uses
// them shows members and virtual calls by name.
// Run it from the Script Manager before ApplyStbcNames.java and ApplyStbcData.java, then save the program.
//@category stbc-decomp
import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.*;

import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;

import java.io.File;
import java.io.FileReader;
import java.util.*;

public class ApplyStbcTypes extends GhidraScript {

	private static final CategoryPath CATEGORY = new CategoryPath("/stbc");
	private DataTypeManager dtm;
	private final Map<String, DataType> ours = new HashMap<>();

	@Override
	public void run() throws Exception {
		// ApplyStbcAll.java passes the file as the script argument; run alone, the script asks for it
		File json = getScriptArgs().length > 0 ? new File(getScriptArgs()[0]) : askFile("stbc-decomp types (build/ghidra_types.json)", "Apply");
		JsonObject types;
		try (FileReader reader = new FileReader(json)) {
			types = JsonParser.parseReader(reader).getAsJsonObject().getAsJsonObject("types");
		}
		dtm = currentProgram.getDataTypeManager();
		int created = 0, fields = 0, skipped = 0, replaced = 0;

		// 1. every type, empty but with its size, so members can point at any of them
		for (Map.Entry<String, JsonElement> e : types.entrySet()) {
			String name = e.getKey();
			JsonObject d = e.getValue().getAsJsonObject();
			int size = d.get("size").getAsInt();
			DataType dt;
			switch (d.get("kind").getAsString()) {
				case "enum":
					EnumDataType en = new EnumDataType(CATEGORY, name, 4, dtm);
					for (JsonElement v : d.getAsJsonArray("values")) {
						JsonObject o = v.getAsJsonObject();
						try {
							en.add(o.get("name").getAsString(), o.get("value").getAsLong());
						}
						catch (IllegalArgumentException ex) {
							// a duplicate name or value: keep the first
						}
					}
					dt = en;
					break;
				case "union":
					dt = new UnionDataType(CATEGORY, name, dtm);
					break;
				default:
					dt = new StructureDataType(CATEGORY, name, Math.max(size, 0), dtm);
			}
			ours.put(name, dtm.addDataType(dt, DataTypeConflictHandler.REPLACE_HANDLER));
			created++;
		}

		// 2. the members
		for (Map.Entry<String, JsonElement> e : types.entrySet()) {
			if (monitor.isCancelled()) {
				break;
			}
			JsonObject d = e.getValue().getAsJsonObject();
			DataType dt = ours.get(e.getKey());
			if (!(dt instanceof Composite)) {
				continue;
			}
			for (JsonElement f : d.getAsJsonArray("fields")) {
				JsonObject o = f.getAsJsonObject();
				String name = fieldName(o.get("name").getAsString());
				DataType ft = resolve(o.getAsJsonObject("type"));
				if (ft == null || ft.getLength() <= 0) {
					skipped++;
					continue;
				}
				try {
					if (dt instanceof Union) {
						((Union) dt).add(ft, name, null);
					}
					else {
						Structure s = (Structure) dt;
						int offset = o.get("offset").getAsInt();
						if (offset + ft.getLength() > s.getLength()) {
							skipped++;
							continue;
						}
						s.replaceAtOffset(offset, ft, ft.getLength(), name, null);
					}
					fields++;
				}
				catch (Exception ex) {
					skipped++;   // an overlapping member (a bitfield, an anonymous union's alternative)
				}
			}
		}

		// 3. the demangler's empty placeholders of the same names give way to these
		List<DataType> placeholders = new ArrayList<>();
		Iterator<Composite> all = dtm.getAllComposites();
		while (all.hasNext()) {
			Composite c = all.next();
			DataType mine = ours.get(c.getName());
			if (mine != null && mine != c && !c.getCategoryPath().equals(CATEGORY) && c.isNotYetDefined()) {
				placeholders.add(c);
			}
		}
		for (DataType old : placeholders) {
			try {
				dtm.replaceDataType(old, ours.get(old.getName()), false);
				replaced++;
			}
			catch (Exception ex) {
				println("could not replace " + old.getPathName() + ": " + ex.getMessage());
			}
		}
		println("types " + created + ", members " + fields + " (skipped " + skipped + "), demangler placeholders replaced " +
			replaced);
	}

	private static String fieldName(String name) {
		return name.startsWith("~") ? "dtor_" + name.substring(1) : name.replaceAll("[^A-Za-z0-9_]", "_");
	}

	private DataType resolve(JsonObject t) {
		switch (t.get("k").getAsString()) {
			case "prim":
				return primitive(t.get("n").getAsString());
			case "ptr": {
				JsonObject to = t.getAsJsonObject("to");
				DataType target = "func".equals(to.get("k").getAsString()) ? function(to.getAsJsonObject("sig")) : resolve(to);
				return new PointerDataType(target, 4, dtm);
			}
			case "arr": {
				DataType elem = resolve(t.getAsJsonObject("of"));
				int bytes = t.get("bytes").getAsInt();
				if (elem == null || elem.getLength() <= 0 || bytes <= 0) {
					return bytes > 0 ? new ArrayDataType(Undefined1DataType.dataType, bytes, 1, dtm) : null;
				}
				return new ArrayDataType(elem, Math.max(1, bytes / elem.getLength()), elem.getLength(), dtm);
			}
			case "named": {
				String name = t.get("n").getAsString();
				DataType dt = ours.get(name);
				if (dt == null) {   // declared but never defined in the source: an empty placeholder to point at
					dt = dtm.addDataType(new StructureDataType(CATEGORY, name, 0, dtm), DataTypeConflictHandler.KEEP_HANDLER);
					ours.put(name, dt);
				}
				return dt;
			}
			case "func":
				return function(t.getAsJsonObject("sig"));
			default:
				return Undefined4DataType.dataType;
		}
	}

	private DataType function(JsonObject sig) {
		FunctionDefinitionDataType f = new FunctionDefinitionDataType(CATEGORY, "fn", dtm);
		if (sig == null) {
			return f;
		}
		DataType ret = resolve(sig.getAsJsonObject("ret"));
		f.setReturnType(ret == null ? VoidDataType.dataType : ret);
		JsonArray args = sig.getAsJsonArray("args");
		List<ParameterDefinition> params = new ArrayList<>();
		boolean member = sig.has("this") && sig.get("this").getAsBoolean();   // the first argument is 'this'
		for (int i = 0; i < args.size(); i++) {
			DataType a = resolve(args.get(i).getAsJsonObject());
			String name = member ? (i == 0 ? "this_" : "p" + i) : "p" + (i + 1);
			params.add(new ParameterDefinitionImpl(name, a == null ? Undefined4DataType.dataType : a, null));
		}
		f.setArguments(params.toArray(new ParameterDefinition[0]));
		try {
			f.setCallingConvention(sig.get("call").getAsString());
		}
		catch (Exception ex) {
			// an unknown convention name: leave the default
		}
		return f;
	}

	private static DataType primitive(String name) {
		switch (name) {
			case "void": return VoidDataType.dataType;
			case "char": return CharDataType.dataType;
			case "schar": return SignedCharDataType.dataType;
			case "uchar": return UnsignedCharDataType.dataType;
			case "wchar": return WideChar16DataType.dataType;
			case "short": return ShortDataType.dataType;
			case "ushort": return UnsignedShortDataType.dataType;
			case "int": return IntegerDataType.dataType;
			case "uint": return UnsignedIntegerDataType.dataType;
			case "long": return LongDataType.dataType;
			case "ulong": return UnsignedLongDataType.dataType;
			case "longlong": return LongLongDataType.dataType;
			case "ulonglong": return UnsignedLongLongDataType.dataType;
			case "float": return FloatDataType.dataType;
			case "double":
			case "longdouble": return DoubleDataType.dataType;
			case "bool": return BooleanDataType.dataType;
			default: return Undefined4DataType.dataType;
		}
	}
}
