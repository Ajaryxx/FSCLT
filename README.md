# FSCLT
A tool to manage your Filesystem

## print command

Command												|Info
----------------------------------------------------|----------------------
fsclt print info version							| prints the current version of this tool
fsclt print info command [COMMAND_NAMES] 			| prints the usage of this/these command(s) (if [ALL], all commands will be listed)
fsclt print list dir [DIRECTORY_NAMES]				| prints out the all folders and files in this directory (if empty, the active directory will be listed)
fsclt print info element [ELEMENT_NAME]				| prints the info of this/these elements


## search command
Command												|Info
----------------------------------------------------|----------------------
fsclt search element [FLAGS] [ELEMENT_NAME]					| searches for an element in the filesystem
fsclt search element word [ELEMENT_NAME(OPTINAL)] [WORD]					| searches for an word in a file
fsclt search element type [TYPE]					| searches for an element with a specific extension
