/*********************************************************
 *                                                       *
 *                       YumEngine                       *
 *                                                       *
 *            This file is free & open source            *
 *        https://github.com/YumStudioHQ/YumEngine       *
 *                          from                         *
 *                         MONOE.                        *
 *                                                       *
 *********************************************************/

#if defined (YUM_SHOULD_NEVER_THROW) and YUM_SHOULD_NEVER_THROW
# if defined (YUM_PROMOTES_WARNINGS) and YUM_PROMOTES_WARNINGS
#   error "This option is highly deprecated (-DYUM_PROMOTES_WARNINGS promotes this warning to an error)"
# elif defined ( YUM_IGNORES_WARNINGS )
# else
#   warning "This option is highly deprecated"
# endif
#endif