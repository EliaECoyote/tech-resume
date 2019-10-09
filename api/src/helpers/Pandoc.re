let runPandoc = (~fromPath: string, ~toPath: string) => {
  let args = {j|-f $fromPath -t $toPath|j};
  Js.log(("executing", {j|pandoc $args|j}));
  let spawnResult = Node.Child_process.spawnSync({j|pandoc $args|j});
  let resultDetails = Node.Child_process.readAs(spawnResult);
  Js.log({j|Exit code: $resultDetails.status|j});
  Js.log(("whole object resultDetails: ", resultDetails));
};