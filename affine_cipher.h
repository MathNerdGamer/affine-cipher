/*
MIT License

Copyright (c) 2019 Math Nerd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#ifndef MATH_NERD_AFFINE_CIPHER_H
#define MATH_NERD_AFFINE_CIPHER_H
#include <algorithm>
#include <array>
#include <random>
#include <sstream>
#include <stdexcept>
#include <tuple>
#include <math_nerd/int_mod.h>

/** \file affine_cipher.h
    \brief A basic Affine Cipher implementation modulo 97.
 */

/** \namespace math_nerd
    \brief Namespace for all of my projects.
 */
namespace math_nerd
{
    /** \namespace math_nerd::affine_cipher
        \brief Namespace for the Affine Cipher implementation.
     */
    namespace affine_cipher
    {
        /** \name Integer modulo 97
         */
        using z97 = int_mod::int_mod<97>;

        /** \name Affine Key
            \brief  First element is the multiplicative part (slope).
                   Second element is the additive part (y-intercept).
         */
        using affine_key = std::pair<z97, z97>;

        /** \namespace math_nerd::affine_cipher::impl_details
            \brief Contains implementation details.
         */
        namespace impl_details
        {
            /** \property ch_table
                \brief The character table.
             */
            constexpr std::array<char, 97> ch_table =
            {
                'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
                '0','1','2','3','4','5','6','7','8','9',' ','~','-','=','!','@','#','$','%','^','&','*','(',')','_','+',
                '[',']',';','\'',',','.','/','{','}',':','"','<','>','?','`', '\\', '|', '\t', '\n'
            };

            /** \fn constexpr char z97_to_char(z97 num)
                \brief Returns the character assigned to that integer modulo 97.
             */
            char z97_to_char(z97 num)
            {
                return ch_table[static_cast<std::size_t>(num.value())];
            }

            /** \fn constexpr z97 char_to_z97(char c)
                \brief Returns the integer modulo 97 assigned to that character.
             */
            z97 char_to_z97(char c)
            {
                return std::distance(std::begin(ch_table), std::find(std::begin(ch_table), std::end(ch_table), c));
            }

            /** \fn bool valid_affine_key(affine_key key)
                \brief Checks if the key is valid, which is when the multiplicative part is non-zero.
             */
            bool valid_affine_key(affine_key key)
            {
                return ( key.first != 0 );
            }

        } // namespace impl_details

        /** \fn affine_key make_key()
            \brief Makes a random (valid) Affine key.
         */
        affine_key make_key()
        {
            std::random_device device;
            std::mt19937 rng(device());

            std::uniform_int_distribution<int> m_dist(1, 96);
            std::uniform_int_distribution<int> b_dist(0, 96);

            return { m_dist(rng), b_dist(rng) };
        }

        /** \fn std::string encrypt(affine_key const &key, std::string pt)
            \brief Encrypts string char-by-char.
         */
        std::string encrypt(affine_key const &key, std::string pt)
        {
            using namespace impl_details;

            if( !valid_affine_key(key) )
            {
                throw std::invalid_argument("Key is invalid. Multiplicative part must be non-zero.");
            }

            std::string ct;

            z97 m = key.first;
            z97 b = key.second;

            for( auto ch : pt )
            {
                z97 x = char_to_z97(ch);

                // y = mx + b
                char cipher_char = z97_to_char( m * x + b );
                ct.push_back(cipher_char);
            }

            return ct;
        }

        /** \fn std::string decrypt(affine_key const &key, std::string ct)
            \brief Decrypts string char-by-char.
         */
        std::string decrypt(affine_key const &key, std::string ct)
        {
            using namespace impl_details;

            if( !valid_affine_key(key) )
            {
                throw std::invalid_argument("Key is invalid.");
            }

            std::string pt;

            z97 m_inv =  key.first.inverse(); // 1/m (mod 97)
            z97 b_inv = -key.second; // -b

            for( auto ch : ct )
            {
                z97 y = char_to_z97(ch);

                // y = mx + b => x = (y - b)/m = (1/m) * (y - b)
                char cipher_char = z97_to_char( m_inv * (y + b_inv) );
                pt.push_back(cipher_char);
            }

            return pt;
        }

    } // namespace affine_cipher

} // namespace math_nerd

/** \mainpage Affine Cipher, modulo 97
    \section gitlab_link GitLab Link
    View the source code at <a href="https://gitlab.com/mathnerd/affine-cipher">GitLab</a>.
 */

#endif // MATH_NERD_AFFINE_CIPHER_H

