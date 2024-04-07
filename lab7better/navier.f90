subroutine navier(Qwe, filename)
    use, intrinsic :: iso_fortran_env, only: dp=>real64
   implicit none

   real(dp), dimension(0:200, 0:90) :: psi = 0.0
   real(dp), dimension(0:200, 0:90) :: zeta = 0.0 ! Fortranowe macierze 10/10

   real(dp) :: delta = 0.01
   real(dp) :: rho = 1
   real(dp) :: mu = 1

   character(len=*) :: filename

   integer :: i1 = 50
   integer :: j1 = 55

   integer :: nx = 200
   integer :: ny = 90
   integer :: IT_MAX = 20000

   real(dp) :: Qwe,  Qwy, omega, gamma
   integer :: j, i, iteracja,  j2


   j2 = j1 + 2


   Qwy = Qwe * ((ny * delta) ** 3 - (j1 * delta) ** 3 &
      - 3 * j1 * delta * (ny * delta) ** 2 &
      + 3 * (j1 * delta) ** 2 * ny * delta) &
      / ((ny * delta) ** 3)

   ! Brzeg A

   do j = j1, ny
      psi(0,j) = (Qwe / (2 * mu)) * &
         ((delta * j)**3 / 3 - 0.5 * (j * delta) ** 2 &
         * (j1 * delta + ny * delta) + j * delta * j1 * delta * ny * delta)
   end do

   ! Brzeg C

   do j = 0, ny
      psi(nx, j) = (Qwy / (2 * mu)) * &
         ((delta * j) ** 3 / 3 - 0.5 * (j * delta) ** 2 * ny * delta) &
         + (Qwe * (j1 * delta) ** 2 * (-delta * j1 + 3 * ny * delta)) &
         / (12 * mu)

   end do

   ! Brzeg B

   do i = 1, nx - 1
      psi(i, ny) = psi(0, ny)
   end do

   ! Brzeg D

   do i = i1, nx - 1
      psi(i,0) = psi(0, j1)
   end do

   ! Brzeg E

   do j = 1, j1
      psi(i1,j) = psi(0,j1)
   end do

   ! Brzeg F

   do i = 1, i1 - 1
      psi(i, j1) = psi(0, j1)
   end do


   do iteracja = 1, IT_MAX
      if (iteracja < 2000) then
         omega = 0
      else
         omega = 1
      end if

      do i = 1, nx - 1
         do j = 1, ny - 1
            if ((i == i1 .and. j <= j1) .or. (j == j1 .and. i <= i1)) then

            else
               psi(i, j) = 0.25 * (psi(i + 1, j) + psi(i - 1, j) &
                  + psi(i, j + 1) + psi(i, j -1) - delta * delta * zeta(i,j))

               zeta(i, j) = 0.25 * (zeta(i + 1, j) + zeta(i - 1, j) + zeta(i, j + 1) + zeta(i, j -1)) &
                  - (omega * rho / (16 * mu)) * ((psi(i, j + 1) - psi(i, j -1)) * (zeta(i + 1, j) - zeta(i -1, j)) &
                  - (psi(i + 1, j) - psi(i - 1, j)) * (zeta(i, j + 1) - zeta(i, j -1)))
            end if
         end do
      end do


      ! WB ZETA

      ! Brzeg A

      do j = j1, ny
         zeta(0, j) = (0.5 * Qwe / mu) * (2 * j * delta - j1 * delta - ny * delta)
      end do

      ! Brzeg C

      do j = 0, ny
         zeta(nx, j) = (0.5 * Qwy / mu) * (2 * j * delta - delta * ny)
      end do

      ! Brzeg B

      do i = 1, nx - 1
         zeta(i, ny) = (2 / (delta * delta)) * (psi(i,ny-1) - psi(i,ny));
      end do

      ! Brzeg D

      do i = i1, nx - 1
         zeta(i,0) = (2 / (delta * delta)) * (psi(i, 1) - psi(i, 0))
      end do

      ! Brzeg E

      do j = 1, j1 - 1
         zeta(i1, j) = (2 / (delta * delta)) * (psi(i1 + 1, j) - psi(i1, j));
      end do

      ! Brzeg F

      do i = 1, i1
         zeta(i, j1) = (2 / (delta * delta)) * (psi(i, j1 + 1) - psi(i, j1));
      end do

      ! Wierzchołek E/F

      zeta(i1, j1) = 0.5 * (zeta(i1 -1, j1) + zeta(i1, j1 - 1));

      gamma = 0

      do i = 1, nx - 1
         gamma = gamma + psi(i +1, j2) + psi(i - 1, j2) + psi(i, j2 + 1) &
            + psi(i, j2 -1) - 4 * psi(i, j2) - delta * delta * zeta(i, j2)
      end do

      print *, 'Gamma is ', gamma, ' at iteration ', iteracja

   end do


   open(1, file = filename, status = 'replace')

   do i=1, nx - 1
      do j = 1, ny - 1
         write(1, *) i * delta, ' ', j * delta, ' ', psi(i, j), ' ', zeta(i, j), ' ', &
          (psi(i,j+1) - psi(i, j-1)) / (2 * delta) , ' ',  -(psi(i+1, j) - psi(i-1,j)) / (2 * delta) 
      end do
      write(1,*) ''
   end do

   close(1)

end subroutine navier

program variables

    real(8) val
    character(len=20) name

    val = -1000
    name = "wyn.dat"

    call navier(val, name)


    val = -4000
    name = "wyn_Qm4000.dat"

    call navier(val, name)

    val = 4000
    name = "wyn_Q4000.dat"

    call navier(val, name)



end program variables
