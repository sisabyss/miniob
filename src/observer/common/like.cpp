#include "like.h"

// Helper function to check if a character is a valid matchable character (i.e., not a single quote).
bool is_valid_char(char c) {
    return c != '\'';
}

int is_like(const char *src, const char *wildcard) {
    // Base case: if the wildcard is empty, source should also be empty for a match.
    if (*wildcard == '\0') {
        return *src == '\0';
    }

    // Case 1: Handle '%' in the wildcard (matches 0 or more characters)
    if (*wildcard == '%') {
        // Try to match '%' with 0 or more characters.
        // Recursively check if remaining wildcard matches src or any suffix of src.
        while (*src != '\0') {
            if (is_like(src, wildcard + 1)) {
                return 1;  // Found a match
            }
            // Move to the next character in src to try different lengths of matches for '%'
            if (is_valid_char(*src)) {
                src++;
            } else {
                break;
            }
        }
        // Check for the case where '%' matches 0 characters
        return is_like(src, wildcard + 1);
    }

    // Case 2: Handle '_' in the wildcard (matches exactly one valid character)
    if (*wildcard == '_') {
        if (*src != '\0' && is_valid_char(*src)) {
            // If src is not empty and is a valid character, try matching the rest
            return is_like(src + 1, wildcard + 1);
        } else {
            return 0;  // No match
        }
    }

    // Case 3: Regular character match (except for single quote)
    if (is_valid_char(*wildcard) && *src == *wildcard) {
        // If both characters match, continue matching the rest
        return is_like(src + 1, wildcard + 1);
    }

    // If characters don't match, return no match
    return 0;
}
