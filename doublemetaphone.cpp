#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <string.h>

// Helper function to check if a character is a vowel
bool isVowel(char c) {
   c = std::tolower(c);
   return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

// Optimized startsWith function using direct character comparison
bool startsWith(const std::string& str, size_t index, const char* prefix) {
   size_t prefix_len = strlen(prefix);
   if (index + prefix_len > str.length()) {
      return false;
   }
   for (size_t i = 0; i < prefix_len; ++i) {
      if (str[index + i] != prefix[i]) {
         return false;
      }
   }
   return true;
}

std::pair<std::string, std::string> doubleMetaphone(std::string word) {
   // Initialize primary and secondary codes with reserved capacity
   std::string primary;
   primary.reserve(4);  // Prevents reallocations up to 4 characters
   std::string secondary;
   secondary.reserve(4);  // Prevents reallocations up to 4 characters
   int length = word.length();
   int current = 0;

   // Handle empty input
   if (length == 0) {
      return {"", ""};
   }

   // Normalize input to uppercase for consistent comparison
   std::transform(word.begin(), word.end(), word.begin(), ::toupper);

   // Handle initial silent consonants
   if (startsWith(word, 0, "GN") || startsWith(word, 0, "KN") ||
       startsWith(word, 0, "PN") || startsWith(word, 0, "WR") ||
       startsWith(word, 0, "PS")) {
      current++;
   }

   // Main processing loop
   while (current < length && (primary.length() < 4 || secondary.length() < 4)) {
      char c = word[current];

      switch (c) {
         case 'A': case 'E': case 'I': case 'O': case 'U':
            if (current == 0) {
               primary += 'A';
               secondary += 'A';
            }
            current++;
            break;

         case 'B':
            primary += 'P';
            secondary += 'P';
            if (current == length - 1 && current > 0 && word[current - 1] == 'M') {
               secondary += 'M';
            }
            current++;
            break;

         case 'C':
            if (current > 1 && !isVowel(word[current - 2]) &&
                startsWith(word, current - 1, "ACH") &&
                (word[current + 2] != 'I' && word[current + 2] != 'E')) {
               primary += 'K';
               secondary += 'K';
               current += 2;
            }
            else if (current == 0 && startsWith(word, current, "CAESAR")) {
               primary += 'S';
               secondary += 'S';
               current += 2;
            }
            else if (startsWith(word, current, "CH")) {
               if (current > 0 && startsWith(word, current, "CHAE")) {
                  primary += 'K';
                  secondary += 'X';
                  current += 2;
               }
               else {
                  primary += 'K';
                  secondary += 'K';
                  current += 2;
               }
            }
            else if (startsWith(word, current, "CI") ||
                     startsWith(word, current, "CE") ||
                     startsWith(word, current, "CY")) {
               primary += 'S';
               secondary += 'S';
               current++;
            }
            else {
               primary += 'K';
               secondary += 'K';
               current++;
            }
            break;

         case 'D':
            if (startsWith(word, current, "DG") &&
                (word[current + 2] == 'I' || word[current + 2] == 'E' || word[current + 2] == 'Y')) {
               primary += 'J';
               secondary += 'J';
               current += 2;
            }
            else {
               primary += 'T';
               secondary += 'T';
               current++;
            }
            break;

         case 'F':
            primary += 'F';
            secondary += 'F';
            current++;
            break;

         case 'G':
            if (startsWith(word, current, "GH")) {
               if (current > 0 && !isVowel(word[current - 1])) {
                  primary += 'K';
                  secondary += 'K';
                  current += 2;
               }
               else if (current == 0) {
                  if (word[current + 2] == 'I') {
                     primary += 'J';
                     secondary += 'J';
                  }
                  else {
                     primary += 'K';
                     secondary += 'K';
                  }
                  current += 2;
               }
               else {
                  current += 2;  // Silent GH
               }
            }
            else if (startsWith(word, current, "GN")) {
               if (current == 0 || (current + 2 == length) ||
                   (current + 2 < length && isVowel(word[current + 2]))) {
                  primary += 'N';
                  secondary += 'N';
                  current += 2;
               }
               else {
                  primary += 'K';
                  secondary += 'N';
                  current += 2;
               }
            }
            else if (current > 0 && word[current - 1] == 'G') {
               primary += 'K';
               secondary += 'K';
               current++;
            }
            else {
               primary += 'K';
               secondary += 'K';
               current++;
            }
            break;

         case 'H':
            if ((current == 0 || isVowel(word[current - 1])) &&
                current + 1 < length && isVowel(word[current + 1])) {
               primary += 'H';
               secondary += 'H';
               current += 2;
            }
            else {
               current++;
            }
            break;

         case 'J':
            primary += 'J';
            secondary += 'J';
            current++;
            break;

         case 'K':
            primary += 'K';
            secondary += 'K';
            current++;
            break;

         case 'L':
            primary += 'L';
            secondary += 'L';
            current++;
            break;

         case 'M':
            primary += 'M';
            secondary += 'M';
            current++;
            break;

         case 'N':
            primary += 'N';
            secondary += 'N';
            current++;
            break;

         case 'P':
            if (startsWith(word, current, "PH")) {
               primary += 'F';
               secondary += 'F';
               current += 2;
            }
            else {
               primary += 'P';
               secondary += 'P';
               current++;
            }
            break;

         case 'Q':
            primary += 'K';
            secondary += 'K';
            current++;
            break;

         case 'R':
            primary += 'R';
            secondary += 'R';
            current++;
            break;

         case 'S':
            if (startsWith(word, current, "SH") ||
                startsWith(word, current, "SIO") ||
                startsWith(word, current, "SIA")) {
               primary += 'X';
               secondary += 'X';
               current += 2;
            }
            else {
               primary += 'S';
               secondary += 'S';
               current++;
            }
            break;

         case 'T':
            if (startsWith(word, current, "TIO")) {
               primary += 'X';
               secondary += 'X';
               current += 3;
            }
            else if (startsWith(word, current, "TH")) {
               primary += '0';
               secondary += 'T';
               current += 2;
            }
            else {
               primary += 'T';
               secondary += 'T';
               current++;
            }
            break;

         case 'V':
            primary += 'F';
            secondary += 'F';
            current++;
            break;

         case 'W':
            if (current == 0 && current + 1 < length && isVowel(word[current + 1])) {
               primary += 'A';
               secondary += 'F';
               current++;
            }
            else {
               primary += 'W';
               secondary += 'W';
               current++;
            }
            break;

         case 'X':
            primary += 'K';
            secondary += 'S';
            current++;
            break;

         case 'Y':
            if (current == 0 && current + 1 < length && isVowel(word[current + 1])) {
               primary += 'Y';
               secondary += 'Y';
               current++;
            }
            else {
               current++;
            }
            break;

         case 'Z':
            primary += 'S';
            secondary += 'S';
            current++;
            break;

         default:
            current++;
            break;
      }
   }

   return {primary, secondary};
}
