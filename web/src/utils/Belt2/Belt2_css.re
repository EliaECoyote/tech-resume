let combine = (classNames: list(option(string))) =>
  classNames
  |> List.map(Belt.Option.getWithDefault(_, ""))
  |> List.filter(className => className != "")
  |> Css.merge;