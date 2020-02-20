/**
 * returns context value from an *Option* context variant value *if found*.
 * Throws otherwise.
 * Used to automatically throw error when trying to
 * access context value *outside* of the context provider
 */
let useSafeContext = context => {
  let context = React.useContext(context);
  switch (context) {
  | Some(context) => context
  | None => raise(Not_found)
  };
};