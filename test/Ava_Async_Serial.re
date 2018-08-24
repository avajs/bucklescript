open Async.Serial;

test("Async.Serial.test", t =>
  t.end_()
);
test_failing("Async.Serial.test_failing", t => {
  let error: Js.Exn.t = [%raw {| new Error("Oh no") |}];
  t.end_(~error, ());
});
after(t => t.end_());
after_always(t => t.end_());
after_each(t => t.end_());
after_each_always(t => t.end_());
before(t => t.end_());
before_each(t => t.end_());
todo("Async.Serial.todo");
