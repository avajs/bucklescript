open Internal;

[@bs.send] external _pass: promiseExecutionContextJS => unit = "pass";
[@bs.send] external _fail: promiseExecutionContextJS => unit = "fail";
[@bs.send]
external _truthy: (promiseExecutionContextJS, bool) => unit = "truthy";
[@bs.send]
external _falsy: (promiseExecutionContextJS, bool) => unit = "falsy";
[@bs.send] external _is: (promiseExecutionContextJS, 'a, 'a) => unit = "is";
[@bs.send] external _not: (promiseExecutionContextJS, 'a, 'a) => unit = "not";
[@bs.send]
external _deepEqual: (promiseExecutionContextJS, 'a, 'a) => unit = "deepEqual";
[@bs.send]
external _notDeepEqual: (promiseExecutionContextJS, 'a, 'a) => unit =
  "notDeepEqual";
[@bs.send]
external _throws:
  (promiseExecutionContextJS, unit => unit) => Js.Promise.t(unit) =
  "throws";
[@bs.send]
external _notThrows:
  (promiseExecutionContextJS, unit => unit) => Js.Promise.t(unit) =
  "notThrows";
[@bs.send]
external _throwsAsync:
  (promiseExecutionContextJS, unit => Js.Promise.t('a)) => Js.Promise.t(unit) =
  "throwsAsync";
[@bs.send]
external _notThrowsAsync:
  (promiseExecutionContextJS, unit => Js.Promise.t('a)) => Js.Promise.t(unit) =
  "notThrowsAsync";
[@bs.send]
external _regex: (promiseExecutionContextJS, string, Js.Re.t) => unit =
  "regex";
[@bs.send]
external _notRegex: (promiseExecutionContextJS, string, Js.Re.t) => unit =
  "notRegex";
[@bs.send]
external _end: (promiseExecutionContextJS, Js.Nullable.t(Js.Exn.t)) => unit =
  "end";

let makePromiseExecutionContext =
    (promiseExecutionContextJS: promiseExecutionContextJS)
    : promiseExecutionContext => {
  pass: () => _pass(promiseExecutionContextJS),
  fail: () => _fail(promiseExecutionContextJS),
  truthy: actual => _truthy(promiseExecutionContextJS, actual),
  falsy: actual => _falsy(promiseExecutionContextJS, actual),
  is: (expected, actual) => _is(promiseExecutionContextJS, actual, expected),
  not: (expected, actual) =>
    _not(promiseExecutionContextJS, actual, expected),
  deepEqual: (expected, actual) =>
    _deepEqual(promiseExecutionContextJS, actual, expected),
  notDeepEqual: (expected, actual) =>
    _notDeepEqual(promiseExecutionContextJS, actual, expected),
  throws: task => _throws(promiseExecutionContextJS, task),
  notThrows: task => _notThrows(promiseExecutionContextJS, task),
  throwsAsync: thrower => _throwsAsync(promiseExecutionContextJS, thrower),
  notThrowsAsync: nonThrower =>
    _notThrowsAsync(promiseExecutionContextJS, nonThrower),
  regex: (regex, content) =>
    _regex(promiseExecutionContextJS, content, regex),
  notRegex: (regex, content) =>
    _notRegex(promiseExecutionContextJS, content, regex),
};
let makePromiseImplementationResultJS:
  promiseImplementationResultJS('a) => promiseImplementationResult('a) =
  result => result;
let makePromiseImplementation = (promiseImplementation, t) =>
  t
  |> makePromiseExecutionContext
  |> promiseImplementation
  |> makePromiseImplementationResultJS;

[@bs.module "ava"]
external _test:
  (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "default";
let test: promiseInterface('a) =
  (message, implementation) =>
    _test(message, implementation |> makePromiseImplementation);

[@bs.module "ava"]
external _testFailing:
  (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "failing";
let testFailing: failingPromiseInterface('a) =
  (message, implementation) =>
    _testFailing(message, implementation |> makePromiseImplementation);

[@bs.module "ava"]
external _testOnly:
  (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "only";
let testOnly: onlyPromiseInterface('a) =
  (message, implementation) =>
    _testOnly(message, implementation |> makePromiseImplementation);

[@bs.module "ava"] [@bs.scope "failing"]
external _testFailingOnly:
  (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "only";
let testFailingOnly: onlyPromiseInterface('a) =
  (message, implementation) =>
    _testFailingOnly(message, implementation |> makePromiseImplementation);

[@bs.module "ava"]
external _testSkip:
  (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "skip";
let testSkip: skipPromiseInterface('a) =
  (message, implementation) =>
    _testSkip(message, implementation |> makePromiseImplementation);

[@bs.module "ava"] [@bs.scope "failing"]
external _testFailingSkip:
  (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "skip";
let testFailingSkip: skipPromiseInterface('a) =
  (message, implementation) =>
    _testFailingSkip(message, implementation |> makePromiseImplementation);

[@bs.module "ava"]
external _after: (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "after";
let after: hookPromiseInterface('a) =
  implementation => _after(implementation |> makePromiseImplementation);

[@bs.module "ava"] [@bs.scope "after"]
external _afterAlways: (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "always";
let afterAlways: hookPromiseInterface('a) =
  implementation => _afterAlways(implementation |> makePromiseImplementation);

[@bs.module "ava"]
external _afterEach: (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "afterEach";
let afterEach: hookPromiseInterface('a) =
  implementation => _after(implementation |> makePromiseImplementation);

[@bs.module "ava"] [@bs.scope "afterEach"]
external _afterEachAlways:
  (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "always";
let afterEachAlways: hookPromiseInterface('a) =
  implementation =>
    _afterEachAlways(implementation |> makePromiseImplementation);

[@bs.module "ava"]
external _before: (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "before";
let before: hookPromiseInterface('a) =
  implementation => _before(implementation |> makePromiseImplementation);

[@bs.module "ava"]
external _beforeEach: (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
  "beforeEach";
let beforeEach: hookPromiseInterface('a) =
  implementation => _beforeEach(implementation |> makePromiseImplementation);

[@bs.module "ava"] external _todo: string => unit = "todo";
let todo: todoDeclaration = message => _todo(message);

module Serial = {
  [@bs.module "ava"]
  external _test:
    (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "serial";
  let test: promiseInterface('a) =
    (message, implementation) =>
      _test(message, implementation |> makePromiseImplementation);

  [@bs.module "ava"]
  external _testFailing:
    (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "failing";
  let testFailing: failingPromiseInterface('a) =
    (message, implementation) =>
      _testFailing(message, implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _testOnly:
    (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "only";
  let testOnly: onlyPromiseInterface('a) =
    (message, implementation) =>
      _testOnly(message, implementation |> makePromiseImplementation);

  /* [@bs.module "ava"] [@bs.scope ("serial", "failing")]
     external _testFailingOnly :
       (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
       "only";
     let testFailingOnly: onlyPromiseInterface('a) =
       (message, implementation) =>
         _testFailingOnly(message, implementation |> makePromiseImplementation); */

  [@bs.module "ava"] [@bs.scope "serial"]
  external _testSkip:
    (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "skip";
  let testSkip: skipPromiseInterface('a) =
    (message, implementation) =>
      _testSkip(message, implementation |> makePromiseImplementation);

  /* [@bs.module "ava"] [@bs.scope ("serial", "failing")]
     external _testFailingSkip:
       (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
       "skip";
     let testFailingSkip: skipPromiseInterface('a) =
       (message, implementation) =>
         _testFailingSkip(message, implementation |> makePromiseImplementation); */

  [@bs.module "ava"] [@bs.scope "serial"]
  external _after: (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "after";
  let after: hookPromiseInterface('a) =
    implementation => _after(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "after")]
  external _afterAlways:
    (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "always";
  let afterAlways: hookPromiseInterface('a) =
    implementation =>
      _afterAlways(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _afterEach: (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "afterEach";
  let afterEach: hookPromiseInterface('a) =
    implementation => _after(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope ("serial", "afterEach")]
  external _afterEachAlways:
    (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "always";
  let afterEachAlways: hookPromiseInterface('a) =
    implementation =>
      _afterEachAlways(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _before: (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "before";
  let before: hookPromiseInterface('a) =
    implementation => _before(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _beforeEach:
    (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "beforeEach";
  let beforeEach: hookPromiseInterface('a) =
    implementation =>
      _beforeEach(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "serial"]
  external _todo: string => unit = "todo";
  let todo: todoDeclaration = message => _todo(message);
};
