open Ava;

testBefore(t => t.pass());
testAfter(t => t.pass());

test("hooks", t => t.pass());

testBeforeEach(t => t.pass());
testAfterEach(t => t.pass());
