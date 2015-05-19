#include "stdafx.h"
#include <string>

void Const_variable()
  {
  const int try_to_change_me = 17;  // means that we cannot change value
  //try_to_change_me = 19;            // Error

  int const* p_const_value = new int(5);  // means that we cannot change value but can change pointer
  p_const_value = &try_to_change_me;      // OK; but we have leak here :)
  // and do not try to delete this pointer now
  //*p_const_value = 10;                    // Error

  int *const p_const_ptr = new int(7);    // means that we have const pointer but we can change value
  *p_const_ptr = 10;                      // OK
  //p_const_ptr = &try_to_change_me;        // Error

  int const *const p_const_ptr_and_val = new int(10); // means that we have const pointer to const value
  //*p_const_ptr_and_val = 10;                      // Error
  //p_const_ptr_and_val = &try_to_change_me;        // Error
  };

class Date
  {
  friend bool operator == (const Date& i_first, const Date& i_second);
  private:
    int m_year;
    int m_month;
    int m_day;

    mutable bool m_cache_valid;
    mutable std::string m_cache;
    void compute_cache() const{} // fill cache

  public:
    void SetYear(int i_year) { m_year = i_year; }
    int  GetYear() const { return m_year; }

    void SetMonth(int i_month) { m_month = i_month; }
    int  GetMonth() const { return m_month; }

    void SetDay(int i_day) { m_day = i_day; }
    int  GetDay() const { return m_day; }

    std::string StringRepr() const 
      {
      if (!m_cache_valid)
        {
        compute_cache();
        m_cache_valid = true;
        }

      return m_cache;
      }
  };

bool operator == (const Date& i_first, const Date& i_second)
  {
  return i_first.m_year == i_second.m_year && i_first.m_month == i_second.m_month && i_first.m_day == i_second.m_day;
  }

void DateTests()
  {
  const Date date;
  int day = date.GetDay();  // OK
  //date.SetDay(10);          // Error

  Date mutable_date;
  day = mutable_date.GetDay();  // OK
  mutable_date.SetDay(10);      // OK

  Date simple_date;
  const Date* p_const_val_date = &simple_date;
  Date* p_date = &simple_date;
  p_date->SetDay(10);
  day = p_const_val_date->GetDay(); // Equals 10
  }

class Doomsday
  {
  private:
    Date m_doomsday;
  public:
    Doomsday(Date& i_date)
      : m_doomsday (i_date)
      {      }

    bool IsItDoomsday(const Date& i_date) const
      {
      return m_doomsday == i_date;
      }
  };

class Human
  {
  private:
    const Doomsday& m_doomsday;

  public:
    Human(const Doomsday& i_doomsday)
      : m_doomsday(i_doomsday)
      {      }
    
    void GoToWork(const Date& i_today)
      {
      if (m_doomsday.IsItDoomsday(i_today))
        throw std::exception("All is bad and the program will crash if anyone catch exception!");
      }
  };

