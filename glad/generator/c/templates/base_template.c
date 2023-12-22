{% import 'template_utils.h' as template_utils with context %}
/**
 * SPDX-License-Identifier: (WTFPL OR CC0-1.0) AND Apache-2.0
 */
{% block includes %}
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
{% if not options.header_only %}
{% block glad_include %}
#include <glad/{{ feature_set.name }}.h>
{% endblock %}
{% endif %}

{% block preimpl %}
{% endblock %}
{% include 'impl_util.c' %}
{% endblock %}

#ifdef __cplusplus
extern "C" {
#endif

#define GLAD_ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))

typedef struct {
    uint16_t first;
    uint16_t second;
} GladAliasPair_t;

static const char *GLAD_{{ feature_set.name|api }}_fn_names[] = {
{% for command in feature_set.commands %}
    /* {{ "{:>4}".format(command.index)}} */ "{{ command.name }}"{% if not loop.last %},{% endif %}{{""}}
{% endfor %}
};

static const char *GLAD_{{ feature_set.name|api }}_ext_names[] = {
{% for extension in feature_set.extensions %}
    /* {{ "{:>4}".format(extension.index)}} */ "{{ extension.name }}"{% if not loop.last %},{% endif %}{{""}}
{% endfor %}
};

{% set global_context = 'glad_' + feature_set.name + '_context' -%}
{% block variables %}
{% if options.mx_global %}
{% call template_utils.zero_initialized() %}Glad{{ feature_set.name|api }}Context {{ global_context }}{% endcall %}
{% endif %}
{% endblock %}
{% block extensions %}
{% if not options.mx %}

{% for extension in chain(feature_set.features, feature_set.extensions) %}
{% call template_utils.protect(extension) %}
int GLAD_{{ extension.name }} = 0;
{% endcall %}
{% endfor %}
{% endif %}
{% endblock %}
{% if not options.mx %}

{% block commands %}
{% for command in feature_set.commands|c_commands %}
{% call template_utils.protect(command) %}
{{ command.name|pfn }} glad_{{ command.name }} = NULL;
{% endcall %}
{% endfor %}
{% endblock %}
{% endif %}

{% block extension_loaders %}
{% for extension, commands in loadable() %}
{% call template_utils.protect(extension) %}
static void glad_{{ spec.name }}_load_{{ extension.name }}({{ template_utils.context_arg(',') }} GLADuserptrloadfunc load, void* userptr) {
    static const uint16_t s_pfnIdx[] = {
{% for command in commands %}
        {{ "{:>4}".format(command.index) }}{% if not loop.last %},{% else %} {% endif %} /* {{ command.name }} */
{% endfor %}
    };
    uint32_t i;
    if(!{{ ('GLAD_' + extension.name)|ctx(name_only=True) }}) return;
    for (i = 0; i < GLAD_ARRAYSIZE(s_pfnIdx); ++i) {
        const uint16_t pfnIdx = s_pfnIdx[i];
        context->pfnArray[pfnIdx] = load(userptr, GLAD_{{ feature_set.name|api}}_fn_names[pfnIdx]);
    }
}

{% endcall %}
{% endfor %}
{% endblock %}
{% block aliasing %}
{% if options.alias %}
static void glad_{{ spec.name }}_resolve_aliases({{ template_utils.context_arg(def='void') }}) {
{%if aliases|length > 0 %}
    static const GladAliasPair_t s_aliases[] = {
{% for command in feature_set.commands|sort(attribute='name') %}
{% if aliases.get(command.name, [])|length > 0 %}
{% call template_utils.protect(command) %}
{% for alias in aliases.get(command.name, [])|reject('equalto', (command.name, command.index)) %}
{% call template_utils.protect(alias) %}
        { {{ "{:>4}".format(command.index) }}, {{ "{:>4}".format(alias[1]) }} }, /* {{ command.name }} and {{ alias[0] }} */
{% endcall %}
{% endfor %}
{% endcall %}
{% endif %}
{% endfor %}
    };
    void **pfnArray = context->pfnArray;
    uint32_t i;

    for (i = 0; i < GLAD_ARRAYSIZE(s_aliases); ++i) {
        const GladAliasPair_t *pAlias = &s_aliases[i];
        if (pfnArray[pAlias->first] == NULL && pfnArray[pAlias->second] != NULL) {
            pfnArray[pAlias->first] = pfnArray[pAlias->second];
        }
    }
{% else %}
{% if options.mx %}
    GLAD_UNUSED(context);
{% endif %}
{% endif %}
}
{% endif %}
{% endblock %}

{% block loader %}
{% endblock %}

{% if options.loader %}
{% block loader_impl %}
{% for api in feature_set.info.apis %}
{% include 'loader/' + api + '.c' %}
{% endfor %}
{% endblock %}
{% endif %}

#ifdef __cplusplus
}
#endif
{% block postimpl %}
{% endblock %}
