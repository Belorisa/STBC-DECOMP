"""Tests of tools/names.py, the name-evidence report for placeholders, and of the readability backlog in
openwork.py (`advisory`, `finish`) and the rule checker's skip of `_$E` initialisers.

    python -m unittest discover -s tools/tests
"""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import decomp  # noqa: E402
import names  # noqa: E402
import openwork  # noqa: E402
import rules  # noqa: E402

HEADER = """
class Base {
public:
	virtual void	Update( void );
};
// a comment with unknown99 in it
class Button : public Base {
public:
	void			SetShip( TGObject *ship ) { unknown138 = ship; }
	TGObject *		GetMenu( void ) { return unknown134; }
	virtual void	VirtualSlot05( int unknown1 );
	float			Unknown5AF4A0( int system );
protected:
	TGObject *		unknown134;		// 0x134
	TGObject *		unknown138;		// 0x138
	char			name[16];
};
"""


def source_index(decls, order=None, bases=None, declared=None, accessors=None, bodies=None):
    return {"decls": decls, "uses": {}, "bases": bases or {}, "declared": declared or {}, "order": order or {},
            "accessors": accessors or {}, "unit_tokens": {}, "bodies": bodies or {}}


def index(src, wrappers=(), wrapper_names=(), scripts=None, gamebryo=None, bsim=None):
    return {"source": src, "wrappers": (list(wrappers), set(wrapper_names)), "scripts": scripts or {},
            "gamebryo": gamebryo or {}, "bsim": bsim or {}}


def decl(kind):
    return (kind, "src/game/Button.h", 1, True, True)


class Parsing(unittest.TestCase):
    def test_blank_keeps_lengths_and_drops_comments_and_strings(self):
        text = 'a = "unknown12"; // unknown34\nb;'
        out = names.blank(text)
        self.assertEqual(len(out), len(text))
        self.assertEqual(names.placeholder_tokens(out), [])

    def test_tokens(self):
        toks = [t for _, t in names.placeholder_tokens("x->unknown1C; Py_Unknown74BBF0(); VirtualSlot12(); "
                                                       "CallVirtualSlot00(); unknownThing;")]
        self.assertEqual(toks, ["unknown1C", "Py_Unknown74BBF0", "VirtualSlot12"])

    def test_kinds(self):
        self.assertEqual(names.kind_of("Unknown5AF4A0"), "function")
        self.assertEqual(names.kind_of("Py_Unknown74BBF0"), "function")
        self.assertEqual(names.kind_of("UnknownClass6A8C50"), "class")
        self.assertEqual(names.kind_of("VirtualSlot05"), "slot")
        self.assertEqual(names.kind_of("unknown134"), "member")

    def test_class_bodies_and_members(self):
        code = names.blank(HEADER)
        spans = {n: (b, s, e) for n, b, s, e in names.class_bodies(code)}
        self.assertEqual(spans["Button"][0], ["Base"])
        _, s, e = spans["Button"]
        mem = [(k, n) for k, n, _ in names.members(code[s:e])]
        self.assertEqual(mem, [("method", "SetShip"), ("method", "GetMenu"), ("virtual", "VirtualSlot05"),
                               ("method", "Unknown5AF4A0"), ("data", "unknown134"), ("data", "unknown138"),
                               ("data", "name")])

    def test_gamebryo_names_in_the_project_style(self):
        self.assertEqual(names.gamebryo_name("m_kWorldDir"), "worldDir")
        self.assertEqual(names.gamebryo_name("m_fLODAdjust"), "lodAdjust")
        self.assertEqual(names.gamebryo_name("m_spScene"), "scene")
        self.assertEqual(names.gamebryo_name("m_uiActivePlaneFlags"), "activePlaneFlags")
        self.assertEqual(names.lower_first("AlertLevel"), "alertLevel")


class Evidence(unittest.TestCase):
    def test_a_wrapper_calling_the_method_names_it(self):
        src = source_index({("ShipClass", "Unknown5AF4A0"): decl("function")})
        ev = names.evidence(index(src, wrappers=[("ShipClass_DamageSystem", "ShipClass", 10, [
            ("Unknown5AF4A0", "result = (float )arg0->Unknown5AF4A0(arg1,arg2,arg3,arg4);")])],
            scripts={"DamageSystem": 4}))
        (conf, cand, source, text), = ev[("ShipClass", "Unknown5AF4A0")]
        self.assertEqual((conf, cand, source), ("direct", "DamageSystem", "wrapper"))
        self.assertIn("scripts: 4 files", text)

    def test_the_wrapper_resolves_an_inherited_placeholder_to_its_owner(self):
        src = source_index({("Base", "VirtualSlot05"): decl("slot")}, bases={"Derived": ["Base"]})
        ev = names.evidence(index(src, wrappers=[("Derived_Reset", "Derived", 10, [
            ("VirtualSlot05", "arg0->VirtualSlot05();")])]))
        self.assertEqual(ev[("Base", "VirtualSlot05")][0][:2], ("direct", "Reset"))

    def test_a_name_the_class_already_declares_is_a_conflict(self):
        src = source_index({("ShipClass", "Unknown5AF4A0"): decl("function")},
                           declared={"ShipClass": {"DamageSystem"}})
        ev = names.evidence(index(src, wrappers=[("ShipClass_DamageSystem", "ShipClass", 10, [
            ("Unknown5AF4A0", "arg0->Unknown5AF4A0();")])]))
        conf, _, _, text = ev[("ShipClass", "Unknown5AF4A0")][0]
        self.assertEqual(conf, "weak")
        self.assertIn("conflict", text)

    def test_a_wrapper_with_two_placeholders_is_weak(self):
        src = source_index({("A", "Unknown400000"): decl("function"), ("A", "Unknown400100"): decl("function")})
        ev = names.evidence(index(src, wrappers=[("A_Run", "A", 10, [
            ("Unknown400000", "arg0->Unknown400000();"), ("Unknown400100", "arg0->Unknown400100();")])]))
        self.assertEqual({e[0][0] for e in ev.values()}, {"weak"})

    def test_an_evidenced_accessor_names_its_member(self):
        src = source_index({("Button", "unknown138"): decl("member"), ("Button", "unknown134"): decl("member")},
                           accessors={("Button", "unknown138"): [("SetShip", "src/game/Button.h", 9)],
                                      ("Button", "unknown134"): [("GetMenu", "src/game/Button.h", 10)]})
        ev = names.evidence(index(src, wrapper_names={"Button_SetShip"}))
        self.assertEqual(ev[("Button", "unknown138")][0][:3], ("direct", "ship", "accessor"))
        self.assertEqual(ev[("Button", "unknown134")][0][:2], ("weak", "menu"))   # nothing names GetMenu

    def test_a_string_naming_the_method(self):
        src = source_index({("Ship", "Unknown400000"): decl("function")}, bodies={
            ("Ship", "Unknown400000"): ("src/game/Ship.cpp", 5, 'Log( "Ship::Explode: no hull" );')})
        ev = names.evidence(index(src))
        self.assertEqual(ev[("Ship", "Unknown400000")][0][:3], ("direct", "Explode", "string"))

    def test_gamebryo_lineage_aligns_between_shared_names(self):
        src = source_index({("NiCamera", "unknown108"): decl("member")}, order={"NiCamera": [
            ("data", "worldToCam"), ("data", "unknown108"), ("data", "worldLoc")]})
        gb = {"NiCamera": [("data", "worldToCam", "m_aafWorldToCam", "float m_aafWorldToCam[4][4]"),
                           ("data", "viewDist", "m_fViewDist", "float m_fViewDist"),
                           ("data", "worldLoc", "m_kWorldLoc", "NiPoint3 m_kWorldLoc")]}
        ev = names.evidence(index(src, gamebryo=gb))
        self.assertEqual(ev[("NiCamera", "unknown108")][0][:2], ("lineage", "viewDist"))

    def test_lineage_needs_the_same_count_between_anchors(self):
        src = source_index({("NiCamera", "unknown108"): decl("member")}, order={"NiCamera": [
            ("data", "worldToCam"), ("data", "unknown108"), ("data", "worldLoc")]})
        gb = {"NiCamera": [("data", "worldToCam", "", ""), ("data", "a", "", ""), ("data", "b", "", ""),
                           ("data", "worldLoc", "", "")]}
        self.assertNotIn(("NiCamera", "unknown108"), names.evidence(index(src, gamebryo=gb)))

    def test_bsim_of_the_same_class_clearly_ahead(self):
        src = source_index({("NiCamera", "Unknown7E7F70"): decl("function")})
        ev = names.evidence(index(src, bsim={0x7E7F70: [("NiCamera::Clear", 1.0, 19.8),
                                                         ("NiOther::Thing", 0.87, 15.0)]}))
        self.assertEqual(ev[("NiCamera", "Unknown7E7F70")][0][:3], ("lineage", "Clear", "bsim"))
        close = names.evidence(index(src, bsim={0x7E7F70: [("NiCamera::Clear", 0.9, 19.8),
                                                            ("NiOther::Thing", 0.88, 15.0)]}))
        self.assertNotIn(("NiCamera", "Unknown7E7F70"), close)

    def test_parameters_are_their_own_kind(self):
        src = source_index({}, order={"Button": [("virtual", "VirtualSlot05"), ("data", "unknown134")]})
        self.assertEqual(names.key_kind(("Button", "unknown1"), src), "parameter")
        self.assertEqual(names.key_kind(("Button", "unknown134"), src), "member")


def row(addr, unit, status="MATCH", rules_=(), static=False, name="A::F"):
    r = {"addr": "0x%X" % addr, "name": name, "unit": unit, "status": status, "score": 100.0,
         "rules": [{"rule": ru, "tier": tier, "text": text} for ru, tier, text in rules_]}
    if static:
        r["static"] = True
    return r


class Backlog(unittest.TestCase):
    ROWS = [
        row(0x400000, "game/A", rules_=[("R7", "advisory", "placeholder or address-like name unknown1C"),
                                        ("R33", "advisory", "no header comment")]),
        row(0x400100, "game/A", rules_=[("R7", "advisory", "placeholder or address-like name VirtualSlot04")]),
        row(0x400200, "game/A", static=True, rules_=[("R33", "advisory", "no header comment")], name="_$E1"),
        row(0x500000, "game/B", status="DIFF", rules_=[("R7", "advisory", "placeholder or address-like name "
                                                                           "Unknown5AF4A0")]),
        row(0x500100, "game/C", rules_=[("R17", "match", "unused local x")]),
    ]

    def test_placeholder_families(self):
        self.assertEqual(openwork.placeholder_kind("Unknown5AF4A0"), "Unknown<ADDR>")
        self.assertEqual(openwork.placeholder_kind("Py_Unknown74BBF0"), "Unknown<ADDR>")
        self.assertEqual(openwork.placeholder_kind("unknown1C"), "unknown<OFF>")
        self.assertEqual(openwork.placeholder_kind("VirtualSlot04"), "VirtualSlot<NN>")
        self.assertEqual(openwork.placeholder_kind("cubeTable983C68"), "other")

    def test_findings_leave_out_initialisers_and_match_tier(self):
        found = openwork.advisory_findings(self.ROWS, file_level=False)
        self.assertEqual(sorted((u, ru) for _, u, ru, _ in found),
                         [("game/A", "R33"), ("game/A", "R7"), ("game/A", "R7"), ("game/B", "R7")])

    def test_finished_units_have_nothing_open_or_pending(self):
        self.assertEqual(sorted(openwork.finished_units(self.ROWS)), ["game/A"])


class StaticInitialisers(unittest.TestCase):
    def test_an_initialiser_annotation_is_not_checked(self):
        lines = ["// FUNCTION: STBC 0x004736a0 _$E496", "", "class X {", "\tint unknown20;", "};"]
        e = decomp.Entry("FUNCTION", 0x4736A0, "game/X", Path("X.cpp"), 1, None, "_$E496", lines, 4, None)
        self.assertEqual(rules.check_function(e, {}, {}), [])


if __name__ == "__main__":
    unittest.main()
