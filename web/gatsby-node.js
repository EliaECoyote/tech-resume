const MonacoWebpackPlugin = require('monaco-editor-webpack-plugin')

exports.onCreateWebpackConfig = ({
  stage,
  rules,
  loaders,
  plugins,
  actions,
  getConfig,
}) => {
  const config = getConfig()
  // setting monaco-editor alias in order to ship only a subset of the features & languages
  // as described in this issue
  // https://github.com/Microsoft/monaco-editor-webpack-plugin/issues/43#issuecomment-436479402
  config.resolve.alias["monaco-editor"] = "monaco-editor/esm/vs/editor/editor.api"
  const monacoPlugin = new MonacoWebpackPlugin({ languages: ["markdown"] })
  config.plugins = config.plugins.concat(monacoPlugin)
}

const getPathForComponent = path => {
  switch (true) {
    // index pages management
    case /.*\/index.bs\/$/.test(path):
      return path.replace("index.bs/", "")
    // standard page management
    case /\.(bs.js)$/.test(path):
      return path.replace(".bs", "")
    default: 
      return path
  }
}

// inspired by https://github.com/jtberglund/gatsby-plugin-reason/blob/6c2fe7c048b6dc425d2401a06aa92936523e9d0c/src/gatsby-node.js
exports.onCreatePage = async (
  { page: oldPage, actions: { createPage, deletePage } },
) => {
  const path = getPathForComponent(oldPage.path)
  const newPage = { ...oldPage, path }
  deletePage(oldPage)
  createPage(newPage)
}