/*
 * Copyright (c) 2020, Linus Groh <mail@linusgroh.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <AK/Function.h>
#include <AK/String.h>
#include <LibCore/DateTime.h>
#include <LibJS/Runtime/Date.h>
#include <LibJS/Runtime/DatePrototype.h>
#include <LibJS/Runtime/Error.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Value.h>

namespace JS {

static Date* typed_this(VM& vm, GlobalObject& global_object)
{
    auto* this_object = vm.this_value(global_object).to_object(global_object);
    if (!this_object)
        return nullptr;
    if (!is<Date>(this_object)) {
        vm.throw_exception<TypeError>(global_object, ErrorType::NotA, "Date");
        return nullptr;
    }
    return static_cast<Date*>(this_object);
}

DatePrototype::DatePrototype(GlobalObject& global_object)
    : Object(*global_object.object_prototype())
{
}

void DatePrototype::initialize(GlobalObject& global_object)
{
    auto& vm = this->vm();
    Object::initialize(global_object);
    u8 attr = Attribute::Writable | Attribute::Configurable;
    define_native_function(vm.names.getDate, get_date, 0, attr);
    define_native_function(vm.names.getDay, get_day, 0, attr);
    define_native_function(vm.names.getFullYear, get_full_year, 0, attr);
    define_native_function(vm.names.setFullYear, set_full_year, 3, attr);
    define_native_function(vm.names.getHours, get_hours, 0, attr);
    define_native_function(vm.names.setHours, set_hours, 4, attr);
    define_native_function(vm.names.getMilliseconds, get_milliseconds, 0, attr);
    define_native_function(vm.names.setMilliseconds, set_milliseconds, 1, attr);
    define_native_function(vm.names.getMinutes, get_minutes, 0, attr);
    define_native_function(vm.names.setMinutes, set_minutes, 3, attr);
    define_native_function(vm.names.getMonth, get_month, 0, attr);
    define_native_function(vm.names.getSeconds, get_seconds, 0, attr);
    define_native_function(vm.names.setSeconds, set_seconds, 2, attr);
    define_native_function(vm.names.getTime, get_time, 0, attr);
    define_native_function(vm.names.getUTCDate, get_utc_date, 0, attr);
    define_native_function(vm.names.getUTCDay, get_utc_day, 0, attr);
    define_native_function(vm.names.getUTCFullYear, get_utc_full_year, 0, attr);
    define_native_function(vm.names.getUTCHours, get_utc_hours, 0, attr);
    define_native_function(vm.names.getUTCMilliseconds, get_utc_milliseconds, 0, attr);
    define_native_function(vm.names.getUTCMinutes, get_utc_minutes, 0, attr);
    define_native_function(vm.names.getUTCMonth, get_utc_month, 0, attr);
    define_native_function(vm.names.getUTCSeconds, get_utc_seconds, 0, attr);
    define_native_function(vm.names.toDateString, to_date_string, 0, attr);
    define_native_function(vm.names.toGMTString, to_gmt_string, 0, attr);
    define_native_function(vm.names.toISOString, to_iso_string, 0, attr);
    define_native_function(vm.names.toLocaleDateString, to_locale_date_string, 0, attr);
    define_native_function(vm.names.toLocaleString, to_locale_string, 0, attr);
    define_native_function(vm.names.toLocaleTimeString, to_locale_time_string, 0, attr);
    define_native_function(vm.names.toTimeString, to_time_string, 0, attr);
    define_native_function(vm.names.toString, to_string, 0, attr);

    // Aliases.
    define_native_function(vm.names.valueOf, get_time, 0, attr);
    // toJSON() isn't quite an alias for toISOString():
    // - it returns null instead of throwing RangeError
    // - its .length is 1, not 0
    // - it can be transferred to other prototypes
}

DatePrototype::~DatePrototype()
{
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_date)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->date()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_day)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->day()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_full_year)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->full_year()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::set_full_year)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    auto new_year = vm.argument(0).to_i32(global_object);
    if (vm.exception())
        return {};

    auto& datetime = this_object->datetime();

    i32 new_month;
    if (vm.argument_count() >= 2) {
        new_month = vm.argument(1).to_i32(global_object);
        if (vm.exception())
            return {};
    } else {
        new_month = datetime.month();
    }

    i32 new_day;
    if (vm.argument_count() >= 3) {
        new_day = vm.argument(2).to_i32(global_object);
        if (vm.exception())
            return {};
    } else {
        new_day = datetime.day();
    }

    datetime.set_time(new_year, new_month, new_day, datetime.hour(), datetime.minute(), datetime.second());
    return Value(this_object->time());
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_hours)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->hours()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::set_hours)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};

    auto new_hours = vm.argument(0).to_i32(global_object);
    if (vm.exception())
        return {};

    auto& datetime = this_object->datetime();

    i32 new_minutes;
    if (vm.argument_count() >= 2) {
        new_minutes = vm.argument(1).to_i32(global_object);
        if (vm.exception())
            return {};
    } else {
        new_minutes = datetime.minute();
    }

    i32 new_seconds;
    if (vm.argument_count() >= 3) {
        new_seconds = vm.argument(2).to_i32(global_object);
        if (vm.exception())
            return {};
    } else {
        new_seconds = datetime.second();
    }

    if (vm.argument_count() >= 4) {
        auto new_milliseconds = vm.argument(3).to_i32(global_object);
        if (vm.exception())
            return {};
        new_seconds += new_milliseconds / 1000;
        this_object->set_milliseconds(new_milliseconds % 1000);
    }

    datetime.set_time(datetime.year(), datetime.month(), datetime.day(), new_hours, new_minutes, new_seconds);
    return Value(this_object->time());
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_milliseconds)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->milliseconds()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::set_milliseconds)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};

    auto new_milliseconds = vm.argument(0).to_i32(global_object);
    if (vm.exception())
        return {};

    this_object->set_milliseconds(new_milliseconds % 1000);

    auto added_seconds = new_milliseconds / 1000;
    if (added_seconds > 0) {
        auto& datetime = this_object->datetime();
        datetime.set_time(datetime.year(), datetime.month(), datetime.day(), datetime.hour(), datetime.minute(), datetime.second() + added_seconds);
    }

    return Value(this_object->time());
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_minutes)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->minutes()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::set_minutes)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};

    auto new_minutes = vm.argument(0).to_i32(global_object);
    if (vm.exception())
        return {};

    auto& datetime = this_object->datetime();

    i32 new_seconds;
    if (vm.argument_count() >= 2) {
        new_seconds = vm.argument(1).to_i32(global_object);
        if (vm.exception())
            return {};
    } else {
        new_seconds = datetime.second();
    }

    if (vm.argument_count() >= 3) {
        auto new_milliseconds = vm.argument(2).to_i32(global_object);
        if (vm.exception())
            return {};
        new_seconds += new_milliseconds / 1000;
        this_object->set_milliseconds(new_milliseconds % 1000);
    }

    datetime.set_time(datetime.year(), datetime.month(), datetime.day(), datetime.hour(), new_minutes, new_seconds);
    return Value(this_object->time());
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_month)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->month()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_seconds)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->seconds()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::set_seconds)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};

    auto new_seconds = vm.argument(0).to_i32(global_object);
    if (vm.exception())
        return {};

    if (vm.argument_count() >= 2) {
        auto new_milliseconds = vm.argument(1).to_i32(global_object);
        if (vm.exception())
            return {};
        new_seconds += new_milliseconds / 1000;
        this_object->set_milliseconds(new_milliseconds % 1000);
    }

    auto& datetime = this_object->datetime();

    datetime.set_time(datetime.year(), datetime.month(), datetime.day(), datetime.hour(), datetime.minute(), new_seconds);
    return Value(this_object->time());
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_time)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(this_object->time());
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_utc_date)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->utc_date()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_utc_day)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->utc_day()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_utc_full_year)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->utc_full_year()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_utc_hours)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->utc_hours()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_utc_milliseconds)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->utc_milliseconds()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_utc_month)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->utc_month()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_utc_minutes)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->utc_minutes()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::get_utc_seconds)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    return Value(static_cast<double>(this_object->utc_seconds()));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::to_date_string)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    auto string = this_object->date_string();
    return js_string(vm, move(string));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::to_gmt_string)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    auto string = this_object->gmt_date_string();
    return js_string(vm, move(string));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::to_iso_string)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    auto string = this_object->iso_date_string();
    return js_string(vm, move(string));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::to_locale_date_string)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    // FIXME: Optional locales, options params.
    auto string = this_object->locale_date_string();
    return js_string(vm, move(string));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::to_locale_string)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    // FIXME: Optional locales, options params.
    auto string = this_object->locale_string();
    return js_string(vm, move(string));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::to_locale_time_string)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    // FIXME: Optional locales, options params.
    auto string = this_object->locale_time_string();
    return js_string(vm, move(string));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::to_time_string)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    auto string = this_object->time_string();
    return js_string(vm, move(string));
}

JS_DEFINE_NATIVE_FUNCTION(DatePrototype::to_string)
{
    auto* this_object = typed_this(vm, global_object);
    if (!this_object)
        return {};
    auto string = this_object->string();
    return js_string(vm, move(string));
}

}
