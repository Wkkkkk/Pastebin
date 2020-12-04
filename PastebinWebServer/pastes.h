// generated by .\sqlite2cpp.py .\pastes.sql ./pastes Pastebin
#ifndef PASTEBIN_PASTES_H
#define PASTEBIN_PASTES_H

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace Pastebin
{
  namespace Pastes_
  {
    struct Shortlink
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "shortlink";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T shortlink;
            T& operator()() { return shortlink; }
            const T& operator()() const { return shortlink; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
    struct ExpirationLengthInMinutes
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "expiration_length_in_minutes";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T expirationLengthInMinutes;
            T& operator()() { return expirationLengthInMinutes; }
            const T& operator()() const { return expirationLengthInMinutes; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::require_insert>;
    };
    struct CreationAt
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "creation_at";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T creationAt;
            T& operator()() { return creationAt; }
            const T& operator()() const { return creationAt; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text>;
    };
    struct PastePath
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "paste_path";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T pastePath;
            T& operator()() { return pastePath; }
            const T& operator()() const { return pastePath; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::require_insert>;
    };
  } // namespace Pastes_

  struct Pastes: sqlpp::table_t<Pastes,
               Pastes_::Shortlink,
               Pastes_::ExpirationLengthInMinutes,
               Pastes_::CreationAt,
               Pastes_::PastePath>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "pastes";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T pastes;
        T& operator()() { return pastes; }
        const T& operator()() const { return pastes; }
      };
    };
  };
} // namespace Pastebin
#endif
