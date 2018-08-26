open Promise.Serial;

test("Promise.Serial.test", t =>
  t.notThrowsAsync(() => Js.Promise.resolve())
);
testFailing("Promise.Serial.testFailing", t =>
  t.notThrowsAsync(() =>
    Js.Promise.make((~resolve as _, ~reject as _) =>
      Js.Exn.raiseError("Oh no")
    )
  )
);
before(_ => Js.Promise.resolve());
beforeEach(_ => Js.Promise.resolve());
after(_ => Js.Promise.resolve());
afterAlways(_ => Js.Promise.resolve());
afterEach(_ => Js.Promise.resolve());
afterEachAlways(_ => Js.Promise.resolve());
todo("Promise.Serial.todo");
