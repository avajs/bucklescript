type passAssertion = unit => unit;
type failAssertion = unit => unit;
type truthyAssertion = bool => unit;
type falsyAssertion = bool => unit;
/* type deepEqualAssertion = 'a.('a, 'a) => unit; */
type notThrowsAssertion('a) = Js.Promise.t('a) => Js.Promise.t(unit);

type assertions = {
  pass: passAssertion,
  fail: failAssertion,
  /* truthy: truthyAssertion,
     falsy: falsyAssertion,
     deepEqual: 'a .('a, 'a) => unit, */
};
type cbAssertions = {
  pass: passAssertion,
  fail: failAssertion,
  /* truthy: truthyAssertion,
     falsy: falsyAssertion,
     deepEqual: 'a .('a, 'a) => unit, */
  cb: (~error: Js.Exn.t=?, unit) => unit,
};
type promiseAssertions = {
  pass: passAssertion,
  fail: failAssertion,
  /* truthy: truthyAssertion,
     falsy: falsyAssertion,
     deepEqual: 'a .('a, 'a) => unit, */
  notThrows: 'a .notThrowsAssertion('a),
};

type executionContext = assertions;
type cbExecutionContext = cbAssertions;
type promiseExecutionContext = promiseAssertions;

type executionContextJS;
[@bs.send] external _pass : executionContextJS => unit = "pass";
[@bs.send] external _fail : executionContextJS => unit = "fail";
[@bs.send] external _end : executionContextJS => unit = "end";
let makeExecutionContext =
    (executionContextJS: executionContextJS)
    : executionContext => {
  pass: () => _pass(executionContextJS),
  fail: () => _fail(executionContextJS),
};
type cbExecutionContextJS;
[@bs.send] external _cb_pass : cbExecutionContextJS => unit = "pass";
[@bs.send] external _cb_fail : cbExecutionContextJS => unit = "fail";
[@bs.send]
external _cb_end : (cbExecutionContextJS, Js.Nullable.t(Js.Exn.t)) => unit =
  "end";
let makeCbExecutionContext =
    (cbExecutionContextJS: cbExecutionContextJS)
    : cbExecutionContext => {
  pass: () => _cb_pass(cbExecutionContextJS),
  fail: () => _cb_fail(cbExecutionContextJS),
  cb: (~error: option(Js.Exn.t)=?, _) =>
    _cb_end(cbExecutionContextJS, Js.Nullable.fromOption(error)),
};
type promiseExecutionContextJS;
[@bs.send] external _promise_pass : promiseExecutionContextJS => unit = "pass";
[@bs.send] external _promise_fail : promiseExecutionContextJS => unit = "fail";
[@bs.send]
external _promise_not_throws :
  (promiseExecutionContextJS, Js.Promise.t('a)) => Js.Promise.t(unit) =
  "notThrows";
[@bs.send]
external _promise_end :
  (promiseExecutionContextJS, Js.Nullable.t(Js.Exn.t)) => unit =
  "end";
let makePromiseExecutionContext =
    (promiseExecutionContextJS: promiseExecutionContextJS)
    : promiseExecutionContext => {
  pass: () => _promise_pass(promiseExecutionContextJS),
  fail: () => _promise_fail(promiseExecutionContextJS),
  notThrows: nonThrower =>
    _promise_not_throws(promiseExecutionContextJS, nonThrower),
};

type implementationResult = unit;
type promiseImplementationResult('a) = Js.Promise.t('a);
type implementationResultJS = unit;
type promiseImplementationResultJS('a) = Js.Promise.t('a);
let makeImplementationResultJS: implementationResultJS => implementationResult =
  result => result;
let makePromiseImplementationResultJS:
  promiseImplementationResultJS('a) => promiseImplementationResult('a) =
  result => result;

type implementation = executionContext => implementationResult;
type cbImplementation = cbExecutionContext => implementationResult;
type promiseImplementation('a) =
  promiseExecutionContext => promiseImplementationResult('a);

type implementationJS = executionContextJS => implementationResultJS;
type cbImplementationJS = cbExecutionContextJS => implementationResultJS;
let makeImplementation = (implementation, t) =>
  t |> makeExecutionContext |> implementation |> makeImplementationResultJS;
let makeCbImplementation = (cbImplementation, t) =>
  t |> makeCbExecutionContext |> cbImplementation |> makeImplementationResultJS;
let makePromiseImplementation = (promiseImplementation, t) =>
  t
  |> makePromiseExecutionContext
  |> promiseImplementation
  |> makePromiseImplementationResultJS;

type testInterface = (string, implementation) => unit;
type todoDeclaration = string => unit;
type onlyInterface = (string, implementation) => unit;
type skipInterface = (string, implementation) => unit;
type failingInterface = (string, implementation) => unit;
type serialInterface = (string, implementation) => unit;
type afterInterface = implementation => unit;
type beforeInterface = implementation => unit;
type cbInterface = (string, cbImplementation) => unit;
type alwaysInterface = cbImplementation => unit;
type hookCbInterface = cbImplementation => unit;
type hookCbSkipInterface = cbImplementation => unit;
type hookSkipInterface = implementation => unit;
type cbFailingInterface = (string, cbImplementation) => unit;
type cbOnlyInterface = (string, cbImplementation) => unit;
type cbSkipInterface = (string, cbImplementation) => unit;
type promiseInterface('a) = (string, promiseImplementation('a)) => unit;
type alwaysPromiseInterface('a) = promiseImplementation('a) => unit;
type hookPromiseInterface('a) = promiseImplementation('a) => unit;
type hookPromiseSkipPromiseInterface('a) = promiseImplementation('a) => unit;
type hookSkipPromiseInterface('a) = promiseImplementation('a) => unit;
type failingPromiseInterface('a) =
  (string, promiseImplementation('a)) => unit;
type onlyPromiseInterface('a) = (string, promiseImplementation('a)) => unit;
type skipPromiseInterface('a) = (string, promiseImplementation('a)) => unit;

module Sync = {
  [@bs.module "ava"]
  external _test : (string, executionContextJS => unit) => unit = "test";
  let test: testInterface =
    (message, implementation) =>
      _test(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _test_failing : (string, executionContextJS => unit) => unit =
    "failing";
  let test_failing: failingInterface =
    (message, implementation) =>
      _test_failing(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _test_only : (string, executionContextJS => unit) => unit = "only";
  let test_only: onlyInterface =
    (message, implementation) =>
      _test_only(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "failing")]
  external _test_failing_only : (string, executionContextJS => unit) => unit =
    "only";
  let test_failing_only: onlyInterface =
    (message, implementation) =>
      _test_failing_only(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _test_skip : (string, executionContextJS => unit) => unit = "skip";
  let test_skip: skipInterface =
    (message, implementation) =>
      _test_skip(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "failing")]
  external _test_failing_skip : (string, executionContextJS => unit) => unit =
    "skip";
  let test_failing_skip: skipInterface =
    (message, implementation) =>
      _test_failing_skip(message, implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _after : (executionContextJS => unit) => unit = "after";
  let after: afterInterface =
    implementation => _after(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "after")]
  external _after_always : (executionContextJS => unit) => unit = "always";
  let after_always: afterInterface =
    implementation => _after_always(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _after_each : (executionContextJS => unit) => unit = "afterEach";
  let after_each: afterInterface =
    implementation => _after(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "afterEach")]
  external _after_each_always : (executionContextJS => unit) => unit =
    "always";
  let after_each_always: afterInterface =
    implementation =>
      _after_each_always(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _before : (executionContextJS => unit) => unit = "before";
  let before: beforeInterface =
    implementation => _before(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "before")]
  external _before_always : (executionContextJS => unit) => unit = "always";
  let before_always: beforeInterface =
    implementation => _before_always(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _before_each : (executionContextJS => unit) => unit = "beforeEach";
  let before_each: beforeInterface =
    implementation => _before_each(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "beforeEach")]
  external _before_each_always : (executionContextJS => unit) => unit =
    "always";
  let before_each_always: beforeInterface =
    implementation =>
      _before_each_always(implementation |> makeImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _todo : string => unit = "todo";
  let todo: todoDeclaration = message => _todo(message);

  module Serial = {
    [@bs.module "ava"] [@bs.scope "test"]
    external _test : (string, executionContextJS => unit) => unit = "serial";
    let test: testInterface =
      (message, implementation) =>
        _test(message, implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _test_failing : (string, executionContextJS => unit) => unit =
      "failing";
    let test_failing: failingInterface =
      (message, implementation) =>
        _test_failing(message, implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _test_only : (string, executionContextJS => unit) => unit =
      "only";
    let test_only: onlyInterface =
      (message, implementation) =>
        _test_only(message, implementation |> makeImplementation);

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "failing")]
       external _test_failing_only : (string, executionContextJS => unit) => unit =
         "only";
       let test_failing_only: onlyInterface =
         (message, implementation) =>
           _test_failing_only(message, implementation |> makeImplementation); */

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _test_skip : (string, executionContextJS => unit) => unit =
      "skip";
    let test_skip: skipInterface =
      (message, implementation) =>
        _test_skip(message, implementation |> makeImplementation);

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "failing")]
       external _test_failing_skip : (string, executionContextJS => unit) => unit =
         "skip";
       let test_failing_skip: skipInterface =
         (message, implementation) =>
           _test_failing_skip(message, implementation |> makeImplementation); */

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _after : (executionContextJS => unit) => unit = "after";
    let after: afterInterface =
      implementation => _after(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "after")]
    external _after_always : (executionContextJS => unit) => unit = "always";
    let after_always: afterInterface =
      implementation => _after_always(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _after_each : (executionContextJS => unit) => unit = "afterEach";
    let after_each: afterInterface =
      implementation => _after(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "afterEach")]
    external _after_each_always : (executionContextJS => unit) => unit =
      "always";
    let after_each_always: afterInterface =
      implementation =>
        _after_each_always(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _before : (executionContextJS => unit) => unit = "before";
    let before: beforeInterface =
      implementation => _before(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "before")]
    external _before_always : (executionContextJS => unit) => unit = "always";
    let before_always: beforeInterface =
      implementation => _before_always(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _before_each : (executionContextJS => unit) => unit =
      "beforeEach";
    let before_each: beforeInterface =
      implementation => _before_each(implementation |> makeImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _todo : string => unit = "todo";
    let todo: todoDeclaration = message => _todo(message);
  };
};

module Async = {
  [@bs.module "ava"] [@bs.scope "test"]
  external _test : (string, cbExecutionContextJS => unit) => unit = "cb";
  let test: cbInterface =
    (message, implementation) =>
      _test(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "cb")]
  external _test_failing : (string, cbExecutionContextJS => unit) => unit =
    "failing";
  let test_failing: cbFailingInterface =
    (message, implementation) =>
      _test_failing(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "cb")]
  external _test_only : (string, cbExecutionContextJS => unit) => unit =
    "only";
  let test_only: cbOnlyInterface =
    (message, implementation) =>
      _test_only(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "cb", "failing")]
  external _test_failing_only : (string, cbExecutionContextJS => unit) => unit =
    "only";
  let test_failing_only: cbOnlyInterface =
    (message, implementation) =>
      _test_failing_only(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "cb")]
  external _test_skip : (string, cbExecutionContextJS => unit) => unit =
    "skip";
  let test_skip: cbSkipInterface =
    (message, implementation) =>
      _test_skip(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "cb", "failing")]
  external _test_failing_skip : (string, cbExecutionContextJS => unit) => unit =
    "skip";
  let test_failing_skip: cbSkipInterface =
    (message, implementation) =>
      _test_failing_skip(message, implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "after")]
  external _after : (cbExecutionContextJS => unit) => unit = "cb";
  let after: hookCbInterface =
    implementation => _after(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "after", "always")]
  external _after_always : (cbExecutionContextJS => unit) => unit = "cb";
  let after_always: hookCbInterface =
    implementation => _after_always(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "afterEach")]
  external _after_each : (cbExecutionContextJS => unit) => unit = "cb";
  let after_each: hookCbInterface =
    implementation => _after(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "afterEach", "always")]
  external _after_each_always : (cbExecutionContextJS => unit) => unit = "cb";
  let after_each_always: hookCbInterface =
    implementation =>
      _after_each_always(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "before")]
  external _before : (cbExecutionContextJS => unit) => unit = "cb";
  let before: hookCbInterface =
    implementation => _before(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "before", "always")]
  external _before_always : (cbExecutionContextJS => unit) => unit = "cb";
  let before_always: hookCbInterface =
    implementation => _before_always(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "beforeEach")]
  external _before_each : (cbExecutionContextJS => unit) => unit = "cb";
  let before_each: hookCbInterface =
    implementation => _before_each(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "beforeEach", "always")]
  external _before_each_always : (cbExecutionContextJS => unit) => unit = "cb";
  let before_each_always: hookCbInterface =
    implementation =>
      _before_each_always(implementation |> makeCbImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _todo : string => unit = "todo";
  let todo: todoDeclaration = message => _todo(message);

  module Serial = {
    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _test : (string, cbExecutionContextJS => unit) => unit = "cb";
    let test: cbInterface =
      (message, implementation) =>
        _test(message, implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "cb")]
    external _test_failing : (string, cbExecutionContextJS => unit) => unit =
      "failing";
    let test_failing: cbFailingInterface =
      (message, implementation) =>
        _test_failing(message, implementation |> makeCbImplementation);

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "only")]
       external _test_only : (string, cbExecutionContextJS => unit) => unit =
         "cb";
       let test_only: cbOnlyInterface =
         (message, implementation) =>
           _test_only(message, implementation |> makeCbImplementation); */

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "failing", "only")]
       external _test_failing_only :
         (string, cbExecutionContextJS => unit) => unit =
         "cb";
       let test_failing_only: cbOnlyInterface =
         (message, implementation) =>
           _test_failing_only(message, implementation |> makeCbImplementation); */

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "skip")]
       external _test_skip : (string, cbExecutionContextJS => unit) => unit =
         "cb";
       let test_skip: cbSkipInterface =
         (message, implementation) =>
           _test_skip(message, implementation |> makeCbImplementation); */

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "failing", "skip")]
       external _test_failing_skip :
         (string, cbExecutionContextJS => unit) => unit =
         "cb";
       let test_failing_skip: cbSkipInterface =
         (message, implementation) =>
           _test_failing_skip(message, implementation |> makeCbImplementation); */

    [@bs.module "ava"] [@bs.scope ("test", "serial", "after")]
    external _after : (cbExecutionContextJS => unit) => unit = "cb";
    let after: hookCbInterface =
      implementation => _after(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "after", "always")]
    external _after_always : (cbExecutionContextJS => unit) => unit = "cb";
    let after_always: hookCbInterface =
      implementation => _after_always(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "afterEach")]
    external _after_each : (cbExecutionContextJS => unit) => unit = "cb";
    let after_each: hookCbInterface =
      implementation => _after(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "afterEach", "always")]
    external _after_each_always : (cbExecutionContextJS => unit) => unit =
      "cb";
    let after_each_always: hookCbInterface =
      implementation =>
        _after_each_always(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "before")]
    external _before : (cbExecutionContextJS => unit) => unit = "cb";
    let before: hookCbInterface =
      implementation => _before(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "before", "always")]
    external _before_always : (cbExecutionContextJS => unit) => unit = "cb";
    let before_always: hookCbInterface =
      implementation =>
        _before_always(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "beforeEach")]
    external _before_each : (cbExecutionContextJS => unit) => unit = "cb";
    let before_each: hookCbInterface =
      implementation => _before_each(implementation |> makeCbImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _todo : string => unit = "todo";
    let todo: todoDeclaration = message => _todo(message);
  };
};

module Promise = {
  [@bs.module "ava"]
  external _test :
    (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "test";
  let test: promiseInterface('a) =
    (message, implementation) =>
      _test(message, implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _test_failing :
    (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "failing";
  let test_failing: failingPromiseInterface('a) =
    (message, implementation) =>
      _test_failing(message, implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _test_only :
    (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "only";
  let test_only: onlyPromiseInterface('a) =
    (message, implementation) =>
      _test_only(message, implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "failing")]
  external _test_failing_only :
    (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "only";
  let test_failing_only: onlyPromiseInterface('a) =
    (message, implementation) =>
      _test_failing_only(
        message,
        implementation |> makePromiseImplementation,
      );

  [@bs.module "ava"] [@bs.scope "test"]
  external _test_skip :
    (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "skip";
  let test_skip: skipPromiseInterface('a) =
    (message, implementation) =>
      _test_skip(message, implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "failing")]
  external _test_failing_skip :
    (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "skip";
  let test_failing_skip: skipPromiseInterface('a) =
    (message, implementation) =>
      _test_failing_skip(
        message,
        implementation |> makePromiseImplementation,
      );

  [@bs.module "ava"] [@bs.scope "test"]
  external _after : (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "after";
  let after: hookPromiseInterface('a) =
    implementation => _after(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "after")]
  external _after_always :
    (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "always";
  let after_always: hookPromiseInterface('a) =
    implementation =>
      _after_always(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _after_each :
    (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "afterEach";
  let after_each: hookPromiseInterface('a) =
    implementation => _after(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "afterEach")]
  external _after_each_always :
    (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "always";
  let after_each_always: hookPromiseInterface('a) =
    implementation =>
      _after_each_always(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _before : (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "before";
  let before: hookPromiseInterface('a) =
    implementation => _before(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "before")]
  external _before_always :
    (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "always";
  let before_always: hookPromiseInterface('a) =
    implementation =>
      _before_always(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _before_each :
    (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "beforeEach";
  let before_each: hookPromiseInterface('a) =
    implementation =>
      _before_each(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope ("test", "beforeEach")]
  external _before_each_always :
    (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
    "always";
  let before_each_always: hookPromiseInterface('a) =
    implementation =>
      _before_each_always(implementation |> makePromiseImplementation);

  [@bs.module "ava"] [@bs.scope "test"]
  external _todo : string => unit = "todo";
  let todo: todoDeclaration = message => _todo(message);

  module Serial = {
    [@bs.module "ava"] [@bs.scope "test"]
    external _test :
      (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "serial";
    let test: promiseInterface('a) =
      (message, implementation) =>
        _test(message, implementation |> makePromiseImplementation);

    [@bs.module "ava"] [@bs.scope "test"]
    external _test_failing :
      (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "failing";
    let test_failing: failingPromiseInterface('a) =
      (message, implementation) =>
        _test_failing(message, implementation |> makePromiseImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _test_only :
      (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "only";
    let test_only: onlyPromiseInterface('a) =
      (message, implementation) =>
        _test_only(message, implementation |> makePromiseImplementation);

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "failing")]
       external _test_failing_only :
         (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
         "only";
       let test_failing_only: onlyPromiseInterface('a) =
         (message, implementation) =>
           _test_failing_only(
             message,
             implementation |> makePromiseImplementation,
           ); */

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _test_skip :
      (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "skip";
    let test_skip: skipPromiseInterface('a) =
      (message, implementation) =>
        _test_skip(message, implementation |> makePromiseImplementation);

    /* [@bs.module "ava"] [@bs.scope ("test", "serial", "failing")]
       external _test_failing_skip :
         (string, promiseExecutionContextJS => Js.Promise.t('a)) => unit =
         "skip";
       let test_failing_skip: skipPromiseInterface('a) =
         (message, implementation) =>
           _test_failing_skip(
             message,
             implementation |> makePromiseImplementation,
           ); */

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _after : (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "after";
    let after: hookPromiseInterface('a) =
      implementation => _after(implementation |> makePromiseImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "after")]
    external _after_always :
      (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "always";
    let after_always: hookPromiseInterface('a) =
      implementation =>
        _after_always(implementation |> makePromiseImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _after_each :
      (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "afterEach";
    let after_each: hookPromiseInterface('a) =
      implementation => _after(implementation |> makePromiseImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "afterEach")]
    external _after_each_always :
      (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "always";
    let after_each_always: hookPromiseInterface('a) =
      implementation =>
        _after_each_always(implementation |> makePromiseImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _before : (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "before";
    let before: hookPromiseInterface('a) =
      implementation => _before(implementation |> makePromiseImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial", "before")]
    external _before_always :
      (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "always";
    let before_always: hookPromiseInterface('a) =
      implementation =>
        _before_always(implementation |> makePromiseImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _before_each :
      (promiseExecutionContextJS => Js.Promise.t('a)) => unit =
      "beforeEach";
    let before_each: hookPromiseInterface('a) =
      implementation =>
        _before_each(implementation |> makePromiseImplementation);

    [@bs.module "ava"] [@bs.scope ("test", "serial")]
    external _todo : string => unit = "todo";
    let todo: todoDeclaration = message => _todo(message);
  };
};
