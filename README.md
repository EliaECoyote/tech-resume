# Tech resume

## Getting started

### Requirements

* vscode editor
* vscode [remote-containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension
* **Windows:** [Docker Desktop](https://www.docker.com/products/docker-desktop) 2.0+ on Windows 10 Pro/Enterprise. (Docker Toolbox is not supported. Windows container images are not supported.)
* **macOS**:  [Docker Desktop](https://www.docker.com/products/docker-desktop) 2.0+.
* **Linux**: [Docker CE/EE](https://docs.docker.com/install/#supported-platforms) 18.06+ and [Docker Compose](https://docs.docker.com/compose/install) 1.21+. (The Ubuntu snap package is not supported.)

### Editor

The issue behind the choice of choosing **vscode editor** as a **requirement** is that reasonML *intellisense and warning / error display (in editor)* uses **artifacts** generated by the bucklescript compiler.\
Since the bucklescript compiler runs and generates artifacts inside a docker container, it's not possible to get the features described above to work correctly.\
The solution right now is to use an extension like [remote-containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers), which allows you to use a Docker container as a full-featured development environment.\
This means that you will get access to the filesystem inside the container, with an already-defined set of vscode extensions.

[Further documentation about remote-container extension](https://code.visualstudio.com/docs/remote/containers)\
[Further documentation about remote-container in a monorepo](https://code.visualstudio.com/docs/remote/containers-advanced#_connecting-to-multiple-containers-at-once)

The steps required to start the *remote-container extension* for a specific module are:

1. open the action menu (cmd + shift + p on macOs)
2. select **Remote-Containers: Open Folder in Container**
3. select the module folder (i.e. web)

Here's a demo:

![Alt Text](/assets/remoteContainerDemo.gif)

⚠️ Warning:

* the 1st time (only) that you run this, it will take a LOT of time, since it will create the docker images for all the services and spawn containers from them
* containers **don't stop** automatically after closing vscode editor - this can be easily done through `docker container stop [CONTAINER_ID]` though

### Logs

All the services logs can be checked through `docker logs --follow [CONTAINER_ID]`. Container ids can be retrieved through
`docker ps` (useful to debug bsb / gatsby / nodemon issues)

## Modules

Single module-related documentation:

* [Web module docs](web/README.md)
* [Api module docs](api/README.md)
* [Pdfgen module docs](pdfgen/README.md)

## CI/CD

Continuous delivery is managed through [google cloudbuild](https://cloud.google.com/cloud-build).\
Commits on master will thus automatically trigger a build => test => deploy pipeline.\
There are 2 kind of cloudbuild pipeline [triggers](https://cloud.google.com/cloud-build/docs/running-builds/create-manage-triggers) implemented:

* **module-specific trigger**: when an update to a *specific module* is detected, the correlated *specific module pipeline* will trigger - no deploy pipeline will be triggered when no change is detected
* **base-image trigger**: when an update to the [base Dockerfile](Dockerfile) is detected, the *build & push base image* pipeline will be triggered