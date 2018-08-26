open Async;

test("Async.test", t => {
  t.pass();
  t.end_();
});
testFailing("Async.testFailing", t => {
  let error: Js.Exn.t = [%raw {| new Error("Oh no") |}];
  t.end_(~error, ());
});
before(t => t.end_());
beforeEach(t => t.end_());
after(t => t.end_());
afterAlways(t => t.end_());
afterEach(t => t.end_());
afterEachAlways(t => t.end_());
todo("Async.todo");
