#ifdef GLAD_GLX

{% if not options.on_demand %}
GLAD_API_CALL int gladLoaderLoadGLX(Display *display, int screen);
{% endif %}

GLAD_API_CALL void gladLoaderUnloadGLX(void);
GLAD_API_CALL void gladLoaderResetGLX{{ 'Context' if options.mx }}({{ template_utils.context_arg(def='void') }});
{% if not options.mx_global %}
GLAD_API_CALL void gladLoaderResetGLX(void);
{% endif %}

#endif