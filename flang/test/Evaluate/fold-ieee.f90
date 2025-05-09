! RUN: %python %S/test_folding.py %s %flang_fc1
module m
  use ieee_arithmetic
  use ieee_exceptions
  logical, parameter :: test_dt_all = ieee_support_datatype()
  logical, parameter :: test_dt_4 = ieee_support_datatype(1.)
  logical, parameter :: test_dt_8 = ieee_support_datatype(1.d0)
  logical, parameter :: test_de_all = ieee_support_denormal()
  logical, parameter :: test_de_4 = ieee_support_denormal(1.)
  logical, parameter :: test_de_8 = ieee_support_denormal(1.d0)
  logical, parameter :: test_di_all = ieee_support_divide()
  logical, parameter :: test_di_4 = ieee_support_divide(1.)
  logical, parameter :: test_di_8 = ieee_support_divide(1.d0)
  logical, parameter :: test_fl_in_all = ieee_support_flag(ieee_invalid)
  logical, parameter :: test_fl_in_4 = ieee_support_flag(ieee_invalid, 1.)
  logical, parameter :: test_fl_in_8 = ieee_support_flag(ieee_invalid, 1.d0)
  logical, parameter :: test_fl_ov_all = ieee_support_flag(ieee_overflow)
  logical, parameter :: test_fl_ov_4 = ieee_support_flag(ieee_overflow, 1.)
  logical, parameter :: test_fl_ov_8 = ieee_support_flag(ieee_overflow, 1.d0)
  logical, parameter :: test_fl_d0_all = ieee_support_flag(ieee_divide_by_zero)
  logical, parameter :: test_fl_d0_4 = ieee_support_flag(ieee_divide_by_zero, 1.)
  logical, parameter :: test_fl_d0_8 = ieee_support_flag(ieee_divide_by_zero, 1.d0)
  logical, parameter :: test_fl_un_all = ieee_support_flag(ieee_underflow)
  logical, parameter :: test_fl_un_4 = ieee_support_flag(ieee_underflow, 1.)
  logical, parameter :: test_fl_un_8 = ieee_support_flag(ieee_underflow, 1.d0)
  logical, parameter :: test_fl_ix_all = ieee_support_flag(ieee_inexact)
  logical, parameter :: test_fl_ix_4 = ieee_support_flag(ieee_inexact, 1.)
  logical, parameter :: test_fl_ix_8 = ieee_support_flag(ieee_inexact, 1.d0)
#if __x86_64__
  logical, parameter :: test_halt_in = ieee_support_halting(ieee_invalid)
  logical, parameter :: test_halt_ov = ieee_support_halting(ieee_overflow)
  logical, parameter :: test_halt_d0 = ieee_support_halting(ieee_divide_by_zero)
  logical, parameter :: test_halt_un = ieee_support_halting(ieee_underflow)
  logical, parameter :: test_halt_ix = ieee_support_halting(ieee_inexact)
#endif
  logical, parameter :: test_inf_all = ieee_support_inf()
  logical, parameter :: test_inf_4 = ieee_support_inf(1.)
  logical, parameter :: test_inf_8 = ieee_support_inf(1.d0)
  logical, parameter :: test_io_all = ieee_support_io()
  logical, parameter :: test_io_4 = ieee_support_io(1.)
  logical, parameter :: test_io_8 = ieee_support_io(1.d0)
  logical, parameter :: test_rd_0_all = ieee_support_rounding(ieee_to_zero)
  logical, parameter :: test_rd_0_4 = ieee_support_rounding(ieee_to_zero, 1.)
  logical, parameter :: test_rd_0_8 = ieee_support_rounding(ieee_to_zero, 1.d0)
  logical, parameter :: test_rd_n_all = ieee_support_rounding(ieee_nearest)
  logical, parameter :: test_rd_n_4 = ieee_support_rounding(ieee_nearest, 1.)
  logical, parameter :: test_rd_n_8 = ieee_support_rounding(ieee_nearest, 1.d0)
  logical, parameter :: test_rd_d_all = ieee_support_rounding(ieee_down)
  logical, parameter :: test_rd_d_4 = ieee_support_rounding(ieee_down, 1.)
  logical, parameter :: test_rd_d_8 = ieee_support_rounding(ieee_down, 1.d0)
  logical, parameter :: test_rd_u_all = ieee_support_rounding(ieee_up)
  logical, parameter :: test_rd_u_4 = ieee_support_rounding(ieee_up, 1.)
  logical, parameter :: test_rd_u_8 = ieee_support_rounding(ieee_up, 1.d0)
  logical, parameter :: test_sq_all = ieee_support_sqrt()
  logical, parameter :: test_sq_4 = ieee_support_sqrt(1.)
  logical, parameter :: test_sq_8 = ieee_support_sqrt(1.d0)
  logical, parameter :: test_sn_all = ieee_support_subnormal()
  logical, parameter :: test_sn_4 = ieee_support_subnormal(1.)
  logical, parameter :: test_sn_8 = ieee_support_subnormal(1.d0)
#if __x86_64__
  logical, parameter :: test_uc_all = .not. ieee_support_underflow_control()
  logical, parameter :: test_uc_4 = ieee_support_underflow_control(1.)
  logical, parameter :: test_uc_8 = ieee_support_underflow_control(1.d0)
  logical, parameter :: test_std_all = ieee_support_standard()
  logical, parameter :: test_std_4 = ieee_support_standard(1.)
  logical, parameter :: test_std_8 = ieee_support_standard(1.d0)
#endif
end
