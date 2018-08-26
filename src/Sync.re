open Internal;

[@bs.send] external _pass: executionContextJS => unit = "pass";
[@bs.send] external _fail: executionContextJS => unit = "fail";
[@bs.send] external _truthy: (executionContextJS, bool) => unit = "truthy";
[@bs.send] external _falsy: (executionContextJS, bool) => unit = "falsy";
[@bs.send] external _is: (executionContextJS, 'a, 'a) => unit = "is";
[@bs.send] external _not: (executionContextJS, 'a, 'a) => unit = "not";
[@bs.send]
external _deepEqual: (executionContextJS, 'a, 'a) => unit = "deepEqual";
[@bs.send]
external _notDeepEqual: (executionContextJS, 'a, 'a) => unit = "notDeepEqual";
[@bs.send]
external _throws: (executionContextJS, unit => unit) => unit = "throws";
[@bs.send]
external _notThrows: (executionContextJS, unit => unit) => unit = "notThrows";
[@bs.send]
external _regex: (executionContextJS, string, Js.Re.t) => unit = "regex";
[@bs.send]
external _notRegex: (executionContextJS, string, Js.Re.t) => unit = "notRegex";
[@bs.send] external _end: executionContextJS => unit = "end";

let makeExecutionContext =
    (executionContextJS: executionContextJS): executionContext => {
  pass: () => _pass(executionContextJS),
  fail: () => _fail(executionContextJS),
  truthy: actual => _truthy(executionContextJS, actual),
  falsy: actual => _falsy(executionContextJS, actual),
  is: (expected, actual) => _is(executionContextJS, actual, expected),
  not: (expected, actual) => _not(executionContextJS, actual, expected),
  deepEqual: (expected, actual) =>
    _deepEqual(executionContextJS, actual, expected),
  notDeepEqual: (expected, actual) =>
    _notDeepEqual(executionContextJS, actual, expected),
  throws: task => _throws(executionContextJS, task),
  notThrows: task => _notThrows(executionContextJS, task),
  regex: (regex, content) => _regex(executionContextJS, content, regex),
  notRegex: (regex, content) =>
    _notRegex(executionContextJS, content, regex),
};
let makeImplementationResultJS: implementationResultJS => implementationResult =
  result => result;
let makeImplementation = (implementation, t) =>
  t |> makeExecutionContext |> implementation |> makeImplementationResultJS;

[@bs.module "ava"]
external _test: (string, executionContextJS => unit) => unit = "default";
let test: testInterface =
  (message, implementation) =>
    _test(message, implementation |> makeImplementation);

[@bs.module "ava"]
external _testFailing: (string, executionContextJS => unit) => unit =
  "failing";
let testFailing: failingInterface =
  (message, implementation) =>
    _testFailing(message, implementation |> makeImplementation);

[@bs.module "ava"]
external _testOnly: (string, executionContextJS => unit) => unit = "only";
let testOnly: onlyInterface =
  (message, implementation) =>
    _testOnly(message, implementation |> makeImplementation);

[@bs.module "ava"] [@bs.scope "failing"]
external _testFailingOnly: (string, executionContextJS => unit) => unit =
  "only";
let testFailingOnly: onlyInterface =
  (message, implementation) =>
    _testFailingOnly(message, implementation |> makeImplementation);

[@bs.module "ava"]
external _testSkip: (string, executionContextJS => unit) => unit = "skip";
let testSkip: skipInterface = (message, _) => _testSkip(message, ignore);

[@bs.module "ava"] [@bs.scope "failing"]
external _testFailingSkip: (string, executionContextJS => unit) => unit =
  "skip";
let testFailingSkip: skipInterface =
  (message, _) => _testFailingSkip(message, ignore);

[@bs.module "ava"]
external _after: (executionContextJS => unit) => unit = "after";
let after: afterInterface =
  implementation => _after(implementation |> makeImplementation);

[@bs.module "ava"] [@bs.scope "after"]
external _afterAlways: (executionContextJS => unit) => unit = "always";
let afterAlways: afterInterface =
  implementation => _afterAlways(implementation |> makeImplementation);

[@bs.module "ava"]
external _afterEach: (executionContextJS => unit) => unit = "afterEach";
let afterEach: afterInterface =
  implementation => _after(implementation |> makeImplementation);

[@bs.module "ava"] [@bs.scope "afterEach"]
external _afterEachAlways: (executionContextJS => unit) => unit = "always";
let afterEachAlways: afterInterface =
  implementation => _afterEachAlways(implementation |> makeImplementation);

[@bs.module "ava"]
external _before: (executionContextJS => unit) => unit = "before";
let before: beforeInterface =
  implementation => _before(implementation |> makeImplementation);

[@bs.module "ava"]
external _beforeEach: (executionContextJS => unit) => unit = "beforeEach";
let beforeEach: beforeInterface =
  implementation => _beforeEach(implementation |> makeImplementation);

[@bs.module "ava"] external _todo: string => unit = "todo";
let todo: todoDeclaration = message => _todo(message);

module Serial = {
  [@bs.module "ava"]
  external _test: (string, executionContextJS => unit) => unit = "serial";
  let test: testInterface =
    (message, implementation) =>
      _test(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _testFailing: (string, executionContextJS => unit) => unit =
    "failing";
  let testFailing: failingInterface =
    (message, implementation) =>
      _testFailing(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _testOnly: (string, executionContextJS => unit) => unit = "only";
  let testOnly: onlyInterface =
    (message, implementation) =>
      _testOnly(message, implementation |> makeImplementation);

  /* [@bs.module "ava"] [@bs.scope ("serial", "failing")]
     external _testFailingOnly : (string, executionContextJS => unit) => unit =
       "only";
     let testFailingOnly: onlyInterface =
       (message, implementation) =>
         _testFailingOnly(message, implementation |> makeImplementation); */

  [@bs.module "ava"] [@bs.scope "serial"]
  external _testSkip: (string, executionContextJS => unit) => unit = "skip";
  let testSkip: skipInterface = (message, _) => _testSkip(message, ignore);

  /* [@bs.module "ava"] [@bs.scope ("serial", "failing")]
     external _testFailingSkip : (string, executionContextJS => unit) => unit =
       "skip";
     let testFailingSkip: skipInterface =
       (message, _) =>
         _testFailingSkip(message, ignore); */

  [@bs.module "ava"] [@bs.scope "serial"]
  external _after: (executionContextJS => unit) => unit = "after";
  let after: afterInterface =
    implementation => _after(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "after")]
  external _afterAlways: (executionContextJS => unit) => unit = "always";
  let afterAlways: afterInterface =
    implementation => _afterAlways(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _afterEach: (executionContextJS => unit) => unit = "afterEach";
  let afterEach: afterInterface =
    implementation => _after(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "afterEach")]
  external _afterEachAlways: (executionContextJS => unit) => unit = "always";
  let afterEachAlways: afterInterface =
    implementation => _afterEachAlways(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _before: (executionContextJS => unit) => unit = "before";
  let before: beforeInterface =
    implementation => _before(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _beforeEach: (executionContextJS => unit) => unit = "beforeEach";
  let beforeEach: beforeInterface =
    implementation => _beforeEach(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _todo: string => unit = "todo";
  let todo: todoDeclaration = message => _todo(message);
};
