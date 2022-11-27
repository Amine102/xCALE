# CI Usage

# Creating A Release

Checkout [this readme](howto/release.md).


# Pipeline

The pipeline has 8 stages but one is disabled (1-Static.Test).

The pipeline is run when a commit to master is made, when a merge request is
made or when when a tag is created.

Release are generated when a tag is created. Package are uploaded on release 
(tag) or on commit to master.

Stage details:
- 0-Prepare: Run by the scheduled pipeline, prepare the environment, the images.
- 1-Static Test: Disabled
- 2-Build: Build for multiple configurations.
- 3-Test: Test the built configurations
- 4-Coverage: Test coverage on a given configuration (Debug by default)
If the tests for pass:
- 5-Packaging: Package the configurations (except Debug by default)
- 6-Publish: Publish to the Gitlab project package registry
- 7-Release: If the commit was tagged, create a new release for that tag.

When published, a packaged configuration (library plus artifacts) is available 
in the package registry. A release references these packages.

When a commit is tagged, the pipeline will create a release, the default 
description for this release is the text specified in the tag message 
(`git tag -a v0.4.0 -m "In version v0.4.0 we changed XX plus XX. the changlog is the following: XXX"`), 
the message may be empty. Try to only tag 
significant version, and keep a clean [version semantic](https://semver.org/).


## Details On Scheduled pipeline

There at least one scheduled pipeline named `Generate CICD environment images`.
It's used to generate fresh images using new packages (more recent compiler / 
lib version, probt version). This pipeline can be run on demand and will also 
be run weekly.

Scheduled pipeline variable for `Generate CICD environment images`:
- A read token for the libprobt repo named `LOCAL_PROBT_CI_READ_USER_TOKEN`.
- `LOCAL_SCHEDULE_CI_IMAGE_GENERATION` set to `1`


# Notes
The path to the `.gitlab-ci.yml` must be set to 
`devops/ci/gitlab/.gitlab-ci.yml` in the CI/CD settings of the Gitlab repo.
