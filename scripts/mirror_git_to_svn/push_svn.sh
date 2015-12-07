#!/bin/sh

repo=qttoolbox

svnurl=http://svn.eol.ucar.edu/svn/$repo

if ! [ $JENKINS_HOME ]; then

    # for testing outside of jenkins
    jenkinsrepo=/tmp/${repo}_jenkins
    giturl=https://github.com/ncareol/$repo.git

    if [ -d $jenkinsrepo ]; then
        cd $jenkinsrepo
        git pull --rebase origin master
    else
        git clone $giturl $jenkinsrepo
        cd $jenkinsrepo
    fi
fi

if ! [ -f .git/config ] || ! grep -F -q svn-remote .git/config; then
    cat << EOD >> .git/config
[svn-remote "svn"]
    url = $svnurl
    fetch = :refs/remotes/git-svn
EOD
fi

env

# create master branch if needed
git show-ref --verify --quiet refs/heads/master || git branch master origin/master

# jenkins does not update master branch, and instead
# works in a DETACHED HEAD state, from a checkout of
# the most recent commit, as returned by
#   git rev-parse origin/master^{commit}
# We could probably do away with this master.
# stuff and use the value of $GIT_COMMIT 
git merge --ff-only origin/master master

# fetch objects from svn-remote named "svn"
# takes a long time the first time it is run from a large repo
git svn fetch svn

# create svn branch tracking git-svn remote
# must be done after above git svn fetch
git show-ref --verify --quiet refs/heads/svn || git branch svn git-svn

# rebase master so it becomes series of commits after HEAD of svn
# takes a long time the first time it is run from a large repo
git rebase svn master

git checkout svn

# fast-forward merge the new commits on master back to svn
git merge --ff-only master

# commit new commits to subversion
git svn dcommit

# set master back to origin/master
git rebase origin/master master

# svn log $svnurl
