Release a Pilgrim General release `vX.Y.Z`

- [ ] Review the list of changes since the last release and record it in a file. This change log should appear in the release tag and potentially in a change log file (there isn't one on Pilgrim for the moment).
- Decide on the version number by reference to
[semver](https://semver.org/).
    * Typically if you want to release code from current `master` branch you will update `MINOR` version, e.g. `0.4.0` -> `0.5.0`.
- [ ] Create an MR (merge request) for [the project](https://gitlab.univ-nantes.fr/PILGRIM/General/-/merge_requests).
    - [ ] Update `CHANGELOG`.
    - [ ] Assign to reviewer (probably yourself for this repository).
- [ ] Once merged create an annotated tag (potentially signed) pointing to the **merge commit** on the **stable branch**

    Note that you can do this using the [gitlab interface](https://gitlab.univ-nantes.fr/PILGRIM/General/-/tags) or using the git command line and in case of the `master` branch:
    ```
    $ git fetch origin master
    $ git tag -a -s -m "Release v0.1.0 / details that will appear in the release description on gitlab" v0.1.0 origin/master
    ```
- [ ] Verify that you created the tag properly, you may use gitlab web UI.
- [ ] If the tag was added, a pipeline will start creating the release packages.

