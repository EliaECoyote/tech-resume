type disposeBagT = {
  add: React.callback(XWonka.Types.subscriptionT, unit),
  dispose: React.callback(unit, unit),
};

/**
 * react hook that allows aggregating multiple wonka subscriptions,
 * and triggers automatic disposal upon *dispose fn* calls or *hook unmount*.
 * Useful to trigger async operations inside, for example, *react onClick*
 * callback props, without checking manually inside *XWonka.subscribe* if
 * the component is still alive or not.
 * Usage:
 * ```
 *  let disposeBag = UseDisposeBag.hook();
 *
 *  <button onClick={_ =>
 *      saveAsyncData(data)
 *      |> XWonka.fromPromise
 *      |> XWonka.subscribe(result => {
 *           safelySetState(_ => true);
 *         })
 *      |> disposeBag.add
 *    }
 *  />
 * ```

 */
let hook = () => {
  let subscriptionList: React.Ref.t(list(XWonka.Types.subscriptionT)) =
    React.useRef([]);

  /**
   * dispose manually all added subscriptions
   */
  let dispose =
    React.useCallback0(() => {
      React.Ref.current(subscriptionList)
      |> Belt.List.forEach(_, subscription => subscription.unsubscribe())
    });

  // dispose subscriptions automatically on hook unmount
  React.useEffect0(() => Some(() => dispose()));

  /**
   * add subscription to a disposable *subscriptionList*.
   * Each subscription will be disposed upon hook unmount OR
   * manual invocations to *dispose*
   */
  let add =
    React.useCallback0(subscription => {
      let subscriptionListValue = React.Ref.current(subscriptionList);
      React.Ref.setCurrent(
        subscriptionList,
        [subscription, ...subscriptionListValue],
      );
    });

  {add, dispose};
};