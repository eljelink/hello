local moo = import "moo.jsonnet";
local ns = "dunedaq.hello.namereceiver";
local s = moo.oschema.schema(ns);

local types = {

 //   int4 :    s.number(  "int4",    "i4",          doc="A signed integer of 4 bytes"),
 //   uint4 :   s.number(  "uint4",   "u4",          doc="An unsigned integer of 4 bytes"),
 //   int8 :    s.number(  "int8",    "i8",          doc="A signed integer of 8 bytes"),
 //   uint8 :   s.number(  "uint8",   "u8",          doc="An unsigned integer of 8 bytes"),
 //   float4 :  s.number(  "float4",  "f4",          doc="A float of 4 bytes"),
 //   double8 : s.number(  "double8", "f8",          doc="A double of 8 bytes"),
 //   boolean:  s.boolean( "Boolean",                doc="A boolean"),
 //   string:   s.string(  "String",   		   doc="A string"),   
  
    string : s.string("Name",
                   doc="A string"),

    count : s.number("Count", "i4",
                     doc="A count of not too many things"),

    conf: s.record("Conf", [
                           s.field("nameParameter", self.string, "Name",
                                           doc="Name as the conversation parameter"),
                           s.field("waitBetweenSendsMsec", self.count, 1000,
                                           doc="Millisecs to wait between sending"),
                           ],
                   doc="NameReceiver configuration"),



 //   conf: s.record("Conf", [
 //                          s.field("some_configured_value", self.int4, 999999,
 //                                          doc="This line is where you'd document the value"),
 //                          ],
 //                  doc="This configuration is for developer education only"),

};

moo.oschema.sort_select(types, ns)
