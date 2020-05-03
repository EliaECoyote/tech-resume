let initialize = () => {
  let dbHandle: PGOCaml.monad(PGOCaml.t('any)) = PGOCaml.connect();
  // let query = PGOCa
  PGOCaml.execute();
  // let query = [%pgsql dbh
  //   "execute"
  //   "create table if not exists themes (
  //     id serial primary key,
  //     name varchar(255)
  //   )"
  // ]
};