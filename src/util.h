#ifndef GAM_UTIL
#define GAM_UTIL

/**
 * Compares two version strings of the format "Major.Minor.Patch",
 * "Major.Minor", or just a bare "Major". The version strings do not need to
 * match in format; missing version components will be assumed to be 0.
 *
 * \return -1 if ver1 is lower than ver2.
 * \return 0 if the two versions are equal.
 * \return 1 if ver1 is greater than ver2.
 * \return INT_MAX if there was an error during parsing.
 */
int cmpver(const char* ver1, const char* ver2);

/**
 * Removes all occurrences of a given character from a string.
 *
 * \return A pointer to a new string with all occurrences of \c chr removed.
 * The returned string is dynamically-allocated, and the caller is responsible
 * for freeing it.
 * \return NULL on failure.
 */
char* strstrp(const char* str, char chr);

#endif
