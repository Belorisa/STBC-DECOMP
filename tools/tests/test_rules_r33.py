"""Tests of R33, the header-comment advisory (rules.header_comment_missing): the comment is looked for above the whole
stack of annotation lines, and compiler-generated members and in-class one-liners need none.

    python -m unittest discover -s tools/tests
"""

import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import decomp  # noqa: E402
import rules  # noqa: E402


def missing(text):
    """{annotated address: R33 finding} of a source text."""
    with tempfile.TemporaryDirectory() as d:
        path = Path(d) / "probe.h"
        path.write_text(text, encoding="latin-1")
        out = {e.addr: rules.header_comment_missing(e) for e in decomp.scan_text("game/Probe", path, text)
               if e.kind == "FUNCTION"}
        rules._file_lines_cache.pop(str(path), None)
        return out


class Stacks(unittest.TestCase):
    def test_comment_above_a_template_stack(self):
        text = ("// The NiTMap instances of this unit.\n"
                "// FUNCTION: STBC 0x00401000 ?A@?$T@H@@QAEXXZ\n"
                "// FUNCTION: STBC 0x00401010 ?B@?$T@H@@QAEXXZ\n"
                "// FUNCTION: STBC 0x00401020 ?C@?$T@H@@QAEXXZ\n"
                "// FUNCTION: STBC 0x00401030 ?D@?$T@H@@QAEXXZ\n"
                "// FUNCTION: STBC 0x00401040 ?E@?$T@H@@QAEXXZ\n"
                "\n")
        self.assertEqual(set(missing(text).values()), {False})

    def test_stack_with_notes(self):
        text = ("/*\n=====\nFoo::Bar\n=====\n*/\n"
                "// FUNCTION: STBC 0x00401000 @game/A\n"
                "// PARKED: frame slots\n"
                "// FUNCTION: STBC 0x00401100 @game/B\n"
                "void Foo::Bar( void ) {\n}\n")
        self.assertEqual(set(missing(text).values()), {False})

    def test_bare_stack(self):
        text = ("int x;\n\n"
                "// FUNCTION: STBC 0x00401000 ?A@?$T@H@@QAEXXZ\n"
                "// FUNCTION: STBC 0x00401010 ?B@?$T@H@@QAEXXZ\n"
                "\n")
        self.assertEqual(set(missing(text).values()), {True})

    def test_data_annotation_is_not_a_comment(self):
        text = ("// DATA: STBC 0x00900000\n"
                "// FUNCTION: STBC 0x00401000\n"
                "void Foo( void ) {\n}\n")
        self.assertEqual(missing(text), {0x401000: True})

    def test_plain_function(self):
        self.assertEqual(missing("x;\n\n// FUNCTION: STBC 0x00401000\nvoid Foo( void ) {\n}\n"), {0x401000: True})
        self.assertEqual(missing("// Does foo.\n// FUNCTION: STBC 0x00401000\nvoid Foo( void ) {\n}\n"),
                         {0x401000: False})


class Exempt(unittest.TestCase):
    def test_in_class_one_liner(self):
        text = ("class Foo {\npublic:\n"
                "\t// FUNCTION: STBC 0x00401000\n"
                "\tvirtual char *\tGetClassName( void ) { return \"Foo\"; }\n"
                "};\n")
        self.assertEqual(missing(text), {0x401000: False})

    def test_in_class_multi_line_still_asks(self):
        text = ("class Foo {\npublic:\n"
                "\tint x;\n"
                "\t// FUNCTION: STBC 0x00401000\n"
                "\tint Get( void ) {\n\t\treturn x;\n\t}\n"
                "};\n")
        self.assertEqual(missing(text), {0x401000: True})

    def test_implicit_destructor_above_its_class(self):
        text = ("int x;\n"
                "// FUNCTION: STBC 0x00401000 ??1Foo@@QAE@XZ\n"
                "class Foo : public Bar {\n};\n")
        self.assertEqual(missing(text), {0x401000: False})

    def test_deleting_destructor_above_a_template(self):
        text = ("#include <string.h>\n\n"
                "// FUNCTION: STBC 0x00401000 ??_G?$KeyArray@VK@@@@UAEPAXI@Z @game/A\n"
                "template <class T>\nclass KeyArray {\n};\n")
        self.assertEqual(missing(text), {0x401000: False})

    def test_default_argument_closure(self):
        text = ("template <class T> class NiTArray {\npublic:\n"
                "\t\t// FUNCTION: STBC 0x00401000 ??_F?$NiTArray@H@@QAEXXZ @game/A\n"
                "\t\tNiTArray( unsigned int maxSize_ = 1 );\n};\n")
        self.assertEqual(missing(text), {0x401000: False})


if __name__ == "__main__":
    unittest.main()
