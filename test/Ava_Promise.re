open Ava.Promise;

test("Promise.test", t =>
  t.notThrows(Js.Promise.resolve())
);
exception Oh_no;
test_failing("Promise.test_failing", t =>
  t.notThrows(Js.Promise.reject(Oh_no))
);
after(_ => Js.Promise.resolve());
after_always(_ => Js.Promise.resolve());
after_each(_ => Js.Promise.resolve());
after_each_always(_ => Js.Promise.resolve());
before(_ => Js.Promise.resolve());
before_each(_ => Js.Promise.resolve());
todo("Promise.todo");
