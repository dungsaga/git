#ifndef BRANCH_H
#define BRANCH_H

struct repository;
struct strbuf;

enum branch_track {
	BRANCH_TRACK_UNSPECIFIED = -1,
	BRANCH_TRACK_NEVER = 0,
	BRANCH_TRACK_REMOTE,
	BRANCH_TRACK_ALWAYS,
	BRANCH_TRACK_EXPLICIT,
	BRANCH_TRACK_OVERRIDE
};

extern enum branch_track git_branch_track;

/* Functions for acting on the information about branches. */

/*
 * This sets the branch.<new_ref>.{remote,merge} config settings so that
 * branch 'new_ref' tracks 'orig_ref'. This is called when branches are
 * created, or when branch configs are updated (e.g. with
 * git branch --set-upstream-to).
 */
void setup_tracking(const char *new_ref, const char *orig_ref,
		    enum branch_track track, int quiet, int expand_orig);

/*
 * Creates a new branch, where:
 *
 *   - r is the repository to add a branch to
 *
 *   - name is the new branch name
 *
 *   - start_name is the name of the existing branch that the new branch should
 *     start from
 *
 *   - force enables overwriting an existing (non-head) branch
 *
 *   - clobber_head_ok, when enabled with 'force', allows the currently
 *     checked out (head) branch to be overwritten
 *
 *   - reflog creates a reflog for the branch
 *
 *   - quiet suppresses tracking information
 *
 *   - track causes the new branch to be configured to merge the remote branch
 *     that start_name is a tracking branch for (if any).
 *
 *   - dry_run causes the branch to be validated but not created.
 *
 */
void create_branch(struct repository *r, const char *name,
		   const char *start_name, int force, int clobber_head_ok,
		   int reflog, int quiet, enum branch_track track, int dry_run);

/*
 * Creates a new branch in repository and its submodules (and its
 * submodules, recursively). Besides these exceptions, the parameters
 * function identically to create_branch():
 *
 * - start_name is the name of the ref, in repository r, that the new
 *   branch should start from. In submodules, branches will start from
 *   the respective gitlink commit ids in start_name's tree.
 *
 * - tracking_name is the name used of the ref that will be used to set
 *   up tracking, e.g. origin/main. This is propagated to submodules so
 *   that tracking information will appear as if the branch branched off
 *   tracking_name instead of start_name (which is a plain commit id for
 *   submodules). If omitted, start_name is used for tracking (just like
 *   create_branch()).
 *
 */
void create_branches_recursively(struct repository *r, const char *name,
				 const char *start_name,
				 const char *tracking_name, int force,
				 int reflog, int quiet, enum branch_track track,
				 int dry_run);
/*
 * Check if 'name' can be a valid name for a branch; die otherwise.
 * Return 1 if the named branch already exists; return 0 otherwise.
 * Fill ref with the full refname for the branch.
 */
int validate_branchname(const char *name, struct strbuf *ref);

/*
 * Check if a branch 'name' can be created as a new branch; die otherwise.
 * 'force' can be used when it is OK for the named branch already exists.
 * Return 1 if the named branch already exists; return 0 otherwise.
 * Fill ref with the full refname for the branch.
 */
int validate_new_branchname(const char *name, struct strbuf *ref, int force);

/*
 * Remove information about the merge state on the current
 * branch. (E.g., MERGE_HEAD)
 */
void remove_merge_branch_state(struct repository *r);

/*
 * Remove information about the state of working on the current
 * branch. (E.g., MERGE_HEAD)
 */
void remove_branch_state(struct repository *r, int verbose);

/*
 * Configure local branch "local" as downstream to branch "remote"
 * from remote "origin".  Used by git branch --set-upstream.
 * Returns 0 on success.
 */
#define BRANCH_CONFIG_VERBOSE 01
int install_branch_config(int flag, const char *local, const char *origin, const char *remote);

/*
 * Read branch description
 */
int read_branch_desc(struct strbuf *, const char *branch_name);

/*
 * Check if a branch is checked out in the main worktree or any linked
 * worktree and die (with a message describing its checkout location) if
 * it is.
 */
void die_if_checked_out(const char *branch, int ignore_current_worktree);

/*
 * Update all per-worktree HEADs pointing at the old ref to point the new ref.
 * This will be used when renaming a branch. Returns 0 if successful, non-zero
 * otherwise.
 */
int replace_each_worktree_head_symref(const char *oldref, const char *newref,
				      const char *logmsg);

#endif
