type namespace =
  | Dns
  | Url
  | Custom(string);

module V1 = {
  [@bs.module] external make: unit => string = "uuid/v1";
};

module V3 = {
  [@bs.module] external uuidv3: (string, string) => string = "uuid/v3";
  [@bs.module "uuid/v3"] external dns: string = "DNS";
  [@bs.module "uuid/v3"] external url: string = "URL";

  let make = (value, namespace) =>
    switch (namespace) {
    | Dns => uuidv3(value, dns)
    | Url => uuidv3(value, url)
    | Custom(guid) => uuidv3(value, guid)
    };
};

module V4 = {
  [@bs.module] external make: unit => string = "uuid/v4";
};

module V5 = {
  [@bs.module] external uuidv5: (string, string) => string = "uuid/v5";
  [@bs.module "uuid/v5"] external dns: string = "DNS";
  [@bs.module "uuid/v5"] external url: string = "URL";
  let make = (value, namespace) =>
    switch (namespace) {
    | Dns => uuidv5(value, dns)
    | Url => uuidv5(value, url)
    | Custom(guid) => uuidv5(value, guid)
    };
};

let make = V4.make;