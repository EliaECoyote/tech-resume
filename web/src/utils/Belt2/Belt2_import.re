/**
 * dynamic imports module at the specified path.
 */
[@bs.val]
external import: string => Js.Promise.t('module_) = "import";