/**
 * lazy ref hook.
 * inspired by https://twitter.com/dan_abramov/status/1149120970856075264?s=20
 * thanks Dan
 */
let hook: (unit => 'a) => 'a =
  init => {
    let ref_ = React.useRef(None);

    switch (React.Ref.current(ref_)) {
    | Some(value) => value
    | None =>
      let value = init();
      React.Ref.setCurrent(ref_, Some(value));
      value;
    };
  };