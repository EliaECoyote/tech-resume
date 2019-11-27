let presets = ["babel-preset-gatsby"]

if (process.env.NODE_ENV === "test")
  presets.push([
    "@babel/preset-env",
    {
      modules: "commonjs",
      targets: { node: "current" }
    }
  ])

module.exports = { presets }
