/**
 * HOF that, given a subscription, returns an optional
 * unsubscribtion fn.
 * This is useful for easier unsubscription inside
 * React **useEffect** hooks
 */
let getEffectCleanup = (subscription: Wonka.Types.subscriptionT) =>
  Some(subscription.unsubscribe);