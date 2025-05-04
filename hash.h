#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <cctype> // used for isalnum check 

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        const int base = 36; 
        const int num_letters = 6; 
        const int groups = 5; 
        
        // 64 bit integer array variable 
        unsigned long long w[groups] = {0};
        
        int index = groups - 1; 
        int count = 0; 

        unsigned long long value = 0; // will hold computed value 
        unsigned long long power = 1; // 36^0  

        for (int i = static_cast<int>(k.length()) - 1; i >= 0; --i)
        {
          char c = k[i]; 

          // skip the non alphabetic or numeric characters
          if (!isalnum(c))
          {
            continue; 
          }
          // convert from base 36 to decimal via given formula 
          int number = letterDigitToNumber(c); 
          value += number * power; 
          power *= base; // update next power (36^x)
          count++; 

          // reset values when we reach 6 characters
          if (count == num_letters)
          {
            w[index--] = value; 
            count = 0; 
            value = 0; 
            power = 1; 
          }
        }

        // less than 6 characters
        if (count > 0 && index >= 0)
        {
          w[index] = value; 
        }

        // hash the string using given formula 
        unsigned long long hash_value = 0; 
        for (int i = 0; i < groups; ++i)
        {
          hash_value += rValues[i] * w[i];
        }

      return hash_value; 
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
      // if letter = a-z then convert to 0-25 
      if (std::isalpha(letter))
      {
        letter = tolower(letter);
        return static_cast<HASH_INDEX_T>(letter) - 'a';
      }
      // if letter = 0-9, then convert to 26-35
      else if (std::isdigit(letter))
      {
        return static_cast<HASH_INDEX_T>(letter - '0' + 26);
      } 
      
      return 0; 
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
