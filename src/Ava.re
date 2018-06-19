type testFuncs = {
  pass: string => unit,
  fail: string => unit,
  truthy: (string, bool) => unit,
  falsy: (string, bool) => unit,
};

[@bs.module "ava"]
external _test : (string, testFuncs => unit) => unit = "test";

[@bs.module "ava"] [@bs.scope "test"]
external _testSerial : (string, testFuncs => unit) => unit = "serial";

[@bs.module "ava"] [@bs.scope "test"]
external _testOnly : (string, testFuncs => unit) => unit = "only";

[@bs.module "ava"] [@bs.scope "test"]
external _testSkip : (string, testFuncs => unit) => unit = "skip";

[@bs.module "ava"] [@bs.scope "test"]
external _testTodo : string => unit = "todo";

[@bs.module "ava"] [@bs.scope "test"]
external _testFailing : (string, testFuncs => unit) => unit = "failing";

[@bs.module "ava"] [@bs.scope "test"]
external _testBefore : (testFuncs => _) => unit = "before";

[@bs.module "ava"] [@bs.scope "test"]
external _testAfter : (testFuncs => _) => unit = "after";

[@bs.module "ava"] [@bs.scope "test"]
external _testBeforeEach : (testFuncs => _) => unit = "beforeEach";

[@bs.module "ava"] [@bs.scope "test"]
external _testAfterEach : (testFuncs => _) => unit = "afterEach";

[@bs.send] external _pass : (testFuncs, string) => unit = "pass";
[@bs.send] external _fail : (testFuncs, string) => unit = "fail";
[@bs.send] external _truthy : (testFuncs, bool, string) => unit = "truthy";
[@bs.send] external _falsy : (testFuncs, bool, string) => unit = "falsy";

let _assertFactory = t => {
  pass: s => _pass(t, s),
  fail: s => _fail(t, s),
  truthy: (s, b) => _truthy(t, b, s),
  falsy: (s, b) => _falsy(t, b, s),
};

let test = (name, f) => _test(name, t => f(_assertFactory(t)));

let testSerial = (name, f) => _testSerial(name, t => f(_assertFactory(t)));

let testOnly = (name, f) => _testOnly(name, t => f(_assertFactory(t)));

let testSkip = (name, f) => _testSkip(name, t => f(_assertFactory(t)));

let testFailing = (name, f) =>
  _testFailing(name, t => f(_assertFactory(t)));

let testBefore = f => _testBefore(t => f(_assertFactory(t)));

let testAfter = f => _testAfter(t => f(_assertFactory(t)));

let testBeforeEach = f => _testBeforeEach(t => f(_assertFactory(t)));

let testAfterEach = f => _testAfterEach(t => f(_assertFactory(t)));

let testTodo = name => _testTodo(name);
