local moo = import "moo.jsonnet";
local s = moo.oschema.schema("dunedaq.hello.hellotoyouinfo");

local info = {
    uint8  : s.number("uint8", "u8",
        doc="An unsigned of 8 bytes"),

    info: s.record("Info", [
        s.field("greeting_sentences", self.uint8, 0, doc="Counting received greeting sentences"),
        s.field("number_of_answers", self.uint8, 0, doc="Counting 'Hello to tou' answers"),
    ], doc="Hello to you information"),

};

moo.oschema.sort_select(info)
