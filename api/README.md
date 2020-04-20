# api

**Contains the following libraries and executables:**

```txt
api@0.0.0
│
├─test/
│   name:    TestApi.exe
│   main:    TestApi
│   require: api.lib
│
├─library/
│   library name: api.lib
│   namespace:    Api
│   require:
│
└─executable/
    name:    ApiApp.exe
    main:    ApiApp
    require: api.lib
```

## Developing

```sh
npm install -g esy
git clone <this-repo>
esy install
esy build
```

## Running Binary

After building the project, you can run the main binary that is produced.

```sh
esy x ApiApp.exe
```

## Running Tests

```sh
# Runs the "test" command in `package.json`.
esy test
```
