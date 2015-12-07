#!/bin/sh

repo=qttoolbox

svnurl=http://svn.eol.ucar.edu/svn/$repo

# For testing outside of jenkins
if ! [ $JENKINS_HOME ]; then

    JENKINS_HOME=/tmp/${repo}_jenkins

    [ -d $JENKINS_HOME ] || mkdir $JENKINS_HOME
    cd $JENKINS_HOME

    # emulate how jenkins sets up the working tree
    # jenkins works in a "detached head" state, with no current
    # branch, doing a checkout on a specific commit

    git rev-parse --is-inside-work-tree || git init
    git config remote.origin.url $giturl
    git -c core.askpass=true fetch --tags --progress $giturl '+refs/heads/*:refs/remotes/origin/*'

    GIT_COMMIT=$(git rev-parse origin/master^{commit})
    git checkout -f $GIT_COMMIT

fi

if ! [ -f .git/config ] || ! grep -F -q svn-remote .git/config; then
    cat << EOD >> .git/config
[svn-remote "svn"]
    url = $svnurl
    fetch = :refs/remotes/git-svn
EOD
fi

# fetch objects from svn-remote named "svn"
# takes a long time the first time it is run from a large repo
git svn fetch svn

# create tmp-master branch pointing at latest commit
git show-ref --verify --quiet refs/heads/tmp-master && git branch -D tmp-master
git branch tmp-master $GIT_COMMIT
git checkout tmp-master

# create svn branch, tracking git-svn remote
# must be done after above git svn fetch
git show-ref --verify --quiet refs/heads/svn || git branch svn git-svn

# rebase tmp-master so it becomes series of commits after HEAD of svn
# takes a long time the first time it is run from a large repo
git rebase svn

git checkout svn

# fast-forward merge the new commits on tmp-master to svn
git merge --ff-only tmp-master

# commit new commits to subversion
git svn dcommit

# reset working tree back to original state
git checkout -f $GIT_COMMIT

# delete temporary branch
git branch -D tmp-master

