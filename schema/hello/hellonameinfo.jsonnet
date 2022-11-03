local moo = import "moo.jsonnet";
local s = moo.oschema.schema("dunedaq.hello.hellonameinfo");

local info = {
    uint8  : s.number("uint8", "u8",
        doc="An unsigned of 8 bytes"),

    info: s.record("Info", [
        s.field("parameter_names", self.uint8, 0, doc="Counting received name configuration parameters"),
        s.field("greeting_sentences", self.uint8, 0, doc="Counting sent greeting sentences"),
    ], doc="Hello name information"),

};

moo.oschema.sort_select(info)
