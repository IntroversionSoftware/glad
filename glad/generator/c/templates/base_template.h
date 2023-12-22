{% import "template_utils.h" as template_utils with context %}
{% block TODO %}
/**
 * Loader generated by glad {{ gen_info.version }} on {{ gen_info.when }}
 *
 * SPDX-License-Identifier: (WTFPL OR CC0-1.0) AND Apache-2.0
 *
 * Generator: {{ gen_info.generator_name }}
 * Specification: {{ gen_info.specification }}
 * Extensions: {{ gen_info.extensions|length }}
 *
 * APIs:
{% for info in gen_info.info %}
 *  - {{ info }}
{% endfor %}
 *
 * Options:
{% for name, value in gen_info.options.items() %}
 *  - {{ name }} = {{ value }}
{% endfor %}
 *
 * Commandline:
 *    {{ gen_info.commandline }}
 *
 * Online:
 *    {{ gen_info.online }}
 *
 */
{% endblock %}

#ifndef GLAD_{{ feature_set.name|upper }}_H_
#define GLAD_{{ feature_set.name|upper }}_H_

{% block preheader %}
{% endblock %}

{% block header %}
{% endblock %}

{% for api in feature_set.info.apis %}
#define GLAD_{{ api|upper }}
{% endfor %}
{% for option in options %}
{% if options[option] %}
#define GLAD_OPTION_{{ feature_set.name|upper }}_{{ option|upper }}
{% endif %}
{% endfor %}

#ifdef __cplusplus
extern "C" {
#endif

{% block platform %}
{% include 'platform.h' %}
{% endblock %}

{% block enums %}
{{ template_utils.write_enumerations(feature_set.enums) }}
{% endblock %}

{% block types %}
{{ template_utils.write_types(feature_set.types) }}
{% endblock %}

{% block feature_information %}
{{ template_utils.write_feature_information(chain(feature_set.features, feature_set.extensions), with_runtime=not options.mx) }}

{% endblock %}
{% block beforecommands %}
{% endblock %}
{% block commands %}

{{ template_utils.write_function_typedefs(feature_set.commands) }}
{% if not options.mx %}
#ifdef __INTELLISENSE__
{{ template_utils.write_function_definitions(feature_set.commands) }}
#else
{{ template_utils.write_function_declarations(feature_set.commands, debug=options.debug) }}
#endif
{% else %}
typedef struct Glad{{ feature_set.name|api }}Context {
    void* userptr;

{% for extension in feature_set.features %}
    unsigned char {{ extension.name|ctx(member=True) }};
{% endfor %}

    union {
        unsigned char extArray[{{feature_set.extensions|length}}];
        struct {
{% for extension in feature_set.extensions %}
        /* {{ "{:>4}".format(extension.index)}} */ unsigned char {{ extension.name|ctx(member=True) }};
{% endfor %}
        };
    };

    union {
        void *pfnArray[{{ feature_set.commands|length }}];
        struct {
{% for command in feature_set.commands %}
{% call template_utils.protect_pfn(command) %}
        /* {{ "{:>4}".format(command.index) }} */ {{ command.name|pfn }} {{ command.name|ctx(member=True) }};
{% endcall %}
{% endfor %}
        };
    };

{% if options.loader %}
    void* glad_loader_handle;
{% endif %}
} Glad{{ feature_set.name|api }}Context;

{% if options.mx_global %}
GLAD_API_CALL Glad{{ feature_set.name|api }}Context glad_{{ feature_set.name }}_context;

{% for extension in chain(feature_set.features, feature_set.extensions) %}
#define GLAD_{{ extension.name }} (glad_{{ feature_set.name }}_context.{{ extension.name|no_prefix }})
{% endfor %}

#ifdef __INTELLISENSE__
{{ template_utils.write_function_definitions(feature_set.commands) }}
#else
{% for command in feature_set.commands %}
#define {{ command.name }} (glad_{{ feature_set.name }}_context.{{ command.name|no_prefix }})
{% endfor %}
#endif
{% endif %}

{% endif %}
{% endblock %}

{% block declarations %}
{% if options.mx_global %}
GLAD_API_CALL Glad{{ feature_set.name|api }}Context* gladGet{{ feature_set.name|api }}Context(void);
GLAD_API_CALL void gladSet{{ feature_set.name|api }}Context(Glad{{ feature_set.name|api }}Context *context);
{% endif %}

{% if options.debug %}
GLAD_API_CALL void gladSet{{ feature_set.name|api }}PreCallback(GLADprecallback cb);
GLAD_API_CALL void gladSet{{ feature_set.name|api }}PostCallback(GLADpostcallback cb);

GLAD_API_CALL void gladInstall{{ feature_set.name|api }}Debug(void);
GLAD_API_CALL void gladUninstall{{ feature_set.name|api }}Debug(void);
{% endif %}
{% endblock %}

{% block custom_declarations %}
{% endblock %}

{% if options.loader %}
{% block loader_impl %}
{% for api in feature_set.info.apis %}
{% include 'loader/' + api + '.h' %}
{% endfor %}
{% endblock %}
{% endif %}

#ifdef __cplusplus
}
#endif
{% block postheader %}
{% endblock %}
#endif
