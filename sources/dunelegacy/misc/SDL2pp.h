/*
 *  This file is part of Dune Legacy.
 *
 *  Dune Legacy is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Dune Legacy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Dune Legacy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SDL2PP_H
#define SDL2PP_H

#include <memory>
#include <assert.h>
#include <misc/exceptions.h>
#include <misc/unique_or_nonowning_ptr.h>
#include <SDL.h>
#include <SDL_endian.h>
#include <SDL_rwops.h>
#include <fakeSDLmixer.h>

namespace sdl2
{
    namespace implementation
    {
        template<typename T, void(*Delete)(T*)>
        struct deleter
        {
            void operator()(T* object) const { Delete(object); }
        };

        template<typename T, void(*Delete)(T*)>
        using unique_ptr_deleter = std::unique_ptr<T, deleter<T, Delete>>;

        template<typename T, void(*Delete)(T*)>
        using unique_or_nonowning_ptr_deleter = unique_or_nonowning_ptr<T, deleter<T, Delete>>;

        template<typename T, typename TArg, void(*Delete)(TArg*)>
        struct arg_deleter
        {
            void operator()(TArg* object) const { Delete(object); }
        };

        template<typename T, typename TArg, void(*Delete)(TArg*)>
        using unique_ptr_arg_deleter = std::unique_ptr<T, arg_deleter<T, TArg, Delete>>;

        template<typename T, typename TArg, void(*Delete)(TArg*)>
        using unique_or_nonowning_ptr_arg_deleter = unique_or_nonowning_ptr<T, arg_deleter<T, TArg, Delete>>;

        struct RWops_deleter
        {
            void operator()(SDL_RWops *RWops) const { if (RWops) { SDL_RWclose(RWops); } }
        };
    }

    template<typename T>
    using sdl_ptr = implementation::unique_ptr_arg_deleter<T, void, SDL_free>;

    typedef std::unique_ptr<SDL_RWops, implementation::RWops_deleter> RWops_ptr;

    typedef implementation::unique_ptr_deleter<Mix_Chunk, Mix_FreeChunk> mix_chunk_ptr;
}

#endif // SDL2PP_H

