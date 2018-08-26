open Async.Serial;

test("Async.Serial.test", t =>
  t.end_()
);
testFailing("Async.Serial.testFailing", t => {
  let error: Js.Exn.t = [%raw {| new Error("Oh no") |}];
  t.end_(~error, ());
});
before(t => t.end_());
beforeEach(t => t.end_());
after(t => t.end_());
afterAlways(t => t.end_());
afterEach(t => t.end_());
afterEachAlways(t => t.end_());
todo("Async.Serial.todo");
