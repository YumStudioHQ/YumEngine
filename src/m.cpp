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

#ifdef YUM_SHOULD_NEVER_THROW
# ifdef YUM_PROMOTES_WARNINGS
#   error "This option is highly deprecated (-DYUM_PROMOTES_WARNINGS promotes this warning to an error)"
# elif defined ( YUM_IGNORES_WARNINGS )
# else
#   warning "This option is highly deprecated"
# endif
#endif