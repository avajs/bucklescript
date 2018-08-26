open Internal;

[@bs.send] external _cb_pass: cbExecutionContextJS => unit = "pass";
[@bs.send] external _cb_fail: cbExecutionContextJS => unit = "fail";
[@bs.send] external _truthy: (cbExecutionContextJS, bool) => unit = "truthy";
[@bs.send] external _falsy: (cbExecutionContextJS, bool) => unit = "falsy";
[@bs.send] external _is: (cbExecutionContextJS, 'a, 'a) => unit = "is";
[@bs.send] external _not: (cbExecutionContextJS, 'a, 'a) => unit = "not";
[@bs.send]
external _deepEqual: (cbExecutionContextJS, 'a, 'a) => unit = "deepEqual";
[@bs.send]
external _notDeepEqual: (cbExecutionContextJS, 'a, 'a) => unit =
  "notDeepEqual";
[@bs.send]
external _regex: (cbExecutionContextJS, string, Js.Re.t) => unit = "regex";
[@bs.send]
external _notRegex: (cbExecutionContextJS, string, Js.Re.t) => unit =
  "notRegex";
[@bs.send]
external _throws: (cbExecutionContextJS, unit => unit) => unit = "throws";
[@bs.send]
external _notThrows: (cbExecutionContextJS, unit => unit) => unit =
  "notThrows";
[@bs.send]
external _cbEnd: (cbExecutionContextJS, Js.Nullable.t(Js.Exn.t)) => unit =
  "end";

let makeCbExecutionContext =
    (cbExecutionContextJS: cbExecutionContextJS): cbExecutionContext => {
  pass: () => _cb_pass(cbExecutionContextJS),
  fail: () => _cb_fail(cbExecutionContextJS),
  truthy: actual => _truthy(cbExecutionContextJS, actual),
  falsy: actual => _falsy(cbExecutionContextJS, actual),
  is: (expected, actual) => _is(cbExecutionContextJS, actual, expected),
  not: (expected, actual) => _not(cbExecutionContextJS, actual, expected),
  deepEqual: (expected, actual) =>
    _deepEqual(cbExecutionContextJS, actual, expected),
  notDeepEqual: (expected, actual) =>
    _notDeepEqual(cbExecutionContextJS, actual, expected),
  regex: (regex, content) => _regex(cbExecutionContextJS, content, regex),
  notRegex: (regex, content) =>
    _notRegex(cbExecutionContextJS, content, regex),
  throws: task => _throws(cbExecutionContextJS, task),
  notThrows: task => _notThrows(cbExecutionContextJS, task),
  end_: (~error: option(Js.Exn.t)=?, _) =>
    _cbEnd(cbExecutionContextJS, Js.Nullable.fromOption(error)),
};
let makeImplementationResultJS: implementationResultJS => implementationResult =
  result => result;
let makeCbImplementation = (cbImplementation, t) =>
  t |> makeCbExecutionContext |> cbImplementation |> makeImplementationResultJS;

[@bs.module "ava"]
external _test: (string, cbExecutionContextJS => unit) => unit = "cb";
let test: cbInterface =
  (message, implementation) =>
    _test(message, implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope "cb"]
external _testFailing: (string, cbExecutionContextJS => unit) => unit =
  "failing";
let testFailing: cbFailingInterface =
  (message, implementation) =>
    _testFailing(message, implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope "cb"]
external _testOnly: (string, cbExecutionContextJS => unit) => unit = "only";
let testOnly: cbOnlyInterface =
  (message, implementation) =>
    _testOnly(message, implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope ("cb", "failing")]
external _testFailingOnly: (string, cbExecutionContextJS => unit) => unit =
  "only";
let testFailingOnly: cbOnlyInterface =
  (message, implementation) =>
    _testFailingOnly(message, implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope "cb"]
external _testSkip: (string, cbExecutionContextJS => unit) => unit = "skip";
let testSkip: cbSkipInterface =
  (message, implementation) =>
    _testSkip(message, implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope ("cb", "failing")]
external _testFailingSkip: (string, cbExecutionContextJS => unit) => unit =
  "skip";
let testFailingSkip: cbSkipInterface =
  (message, implementation) =>
    _testFailingSkip(message, implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope "after"]
external _after: (cbExecutionContextJS => unit) => unit = "cb";
let after: hookCbInterface =
  implementation => _after(implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope ("after", "always")]
external _afterAlways: (cbExecutionContextJS => unit) => unit = "cb";
let afterAlways: hookCbInterface =
  implementation => _afterAlways(implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope "afterEach"]
external _afterEach: (cbExecutionContextJS => unit) => unit = "cb";
let afterEach: hookCbInterface =
  implementation => _after(implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope ("afterEach", "always")]
external _afterEachAlways: (cbExecutionContextJS => unit) => unit = "cb";
let afterEachAlways: hookCbInterface =
  implementation => _afterEachAlways(implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope "before"]
external _before: (cbExecutionContextJS => unit) => unit = "cb";
let before: hookCbInterface =
  implementation => _before(implementation |> makeCbImplementation);

[@bs.module "ava"] [@bs.scope "beforeEach"]
external _beforeEach: (cbExecutionContextJS => unit) => unit = "cb";
let beforeEach: hookCbInterface =
  implementation => _beforeEach(implementation |> makeCbImplementation);

[@bs.module "ava"] external _todo: string => unit = "todo";
let todo: todoDeclaration = message => _todo(message);

module Serial = {
  [@bs.module "ava"] [@bs.scope "serial"]
  external _test: (string, cbExecutionContextJS => unit) => unit = "cb";
  let test: cbInterface =
    (message, implementation) =>
      _test(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "cb")]
  external _testFailing: (string, cbExecutionContextJS => unit) => unit =
    "failing";
  let testFailing: cbFailingInterface =
    (message, implementation) =>
      _testFailing(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "cb")]
  external _testOnly: (string, cbExecutionContextJS => unit) => unit = "only";
  let testOnly: cbOnlyInterface =
    (message, implementation) =>
      _testOnly(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "cb", "failing")]
  external _testFailingOnly: (string, cbExecutionContextJS => unit) => unit =
    "only";
  let testFailingOnly: cbOnlyInterface =
    (message, implementation) =>
      _testFailingOnly(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "cb")]
  external _testSkip: (string, cbExecutionContextJS => unit) => unit = "skip";
  let testSkip: cbSkipInterface =
    (message, implementation) =>
      _testSkip(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "cb", "failing")]
  external _testFailingSkip: (string, cbExecutionContextJS => unit) => unit =
    "skip";
  let testFailingSkip: cbSkipInterface =
    (message, implementation) =>
      _testFailingSkip(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "after")]
  external _after: (cbExecutionContextJS => unit) => unit = "cb";
  let after: hookCbInterface =
    implementation => _after(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "after", "always")]
  external _afterAlways: (cbExecutionContextJS => unit) => unit = "cb";
  let afterAlways: hookCbInterface =
    implementation => _afterAlways(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "afterEach")]
  external _afterEach: (cbExecutionContextJS => unit) => unit = "cb";
  let afterEach: hookCbInterface =
    implementation => _after(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "afterEach", "always")]
  external _afterEachAlways: (cbExecutionContextJS => unit) => unit = "cb";
  let afterEachAlways: hookCbInterface =
    implementation =>
      _afterEachAlways(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "before")]
  external _before: (cbExecutionContextJS => unit) => unit = "cb";
  let before: hookCbInterface =
    implementation => _before(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "beforeEach")]
  external _beforeEach: (cbExecutionContextJS => unit) => unit = "cb";
  let beforeEach: hookCbInterface =
    implementation => _beforeEach(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _todo: string => unit = "todo";
  let todo: todoDeclaration = message => _todo(message);
};
