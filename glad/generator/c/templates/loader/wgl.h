#ifdef GLAD_WGL

{% if not options.on_demand %}
GLAD_API_CALL int gladLoaderLoadWGL(HDC hdc);
{% endif %}
GLAD_API_CALL void gladLoaderResetWGL{{ 'Context' if options.mx }}({{ template_utils.context_arg(def='void') }});
{% if not options.mx_global %}
GLAD_API_CALL void gladLoaderResetWGL(void);
{% endif %}

#endif