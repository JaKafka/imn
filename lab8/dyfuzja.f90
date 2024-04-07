
program dyfuzjaprog
   use, intrinsic :: iso_fortran_env, only: dp=>real64
   implicit none

   ! Potrzebne zmienne

   real(dp), dimension(0:400, 0:90) :: psi = 0
   real(dp), dimension(0:400, 0:90) :: vx = 0
   real(dp), dimension(0:400, 0:90) :: vy = 0

   real(dp), dimension(0:50) :: ct = 0

   real(dp), dimension(0:400, 0:90) :: unow = 0
   real(dp), dimension(0:400, 0:90) :: unext = 0
   real(dp), dimension(0:400, 0:90) :: upreviter = 0

   real(dp) :: firstterm = 0, secondterm = 0, thirdterm = 0

   integer :: i, j, nx, ny, ri, rj, i1, i2, j1, iteracja, k
   real(dp) :: strread, delta, omega, xa, ya, vmax, dt, vtemp, dyfuzja, ctsum

   integer :: tmax = 50

   real(dp) :: PI=3.14

   character(50) fnamefull
   character(5) trimnumber



   nx = 400
   ny = 90
   i1 = 200
   i2 = 210
   j1 = 50
   delta = 0.01_dp
   omega = 10 * delta
   xa = 0.45
   ya = 0.45
   dyfuzja = 0.0

   ! Wczytaj psi.dat

   open(unit=18, file='psi.dat' , status='old', &
      access ='sequential',form='formatted')

   do i = 1, 401 * 91
      read(18, *) ri, rj, strread
      psi(ri, rj) = strread
   end do

   close(18)

   !
   ! Wyznaczenie pola predkosci
   !

   do i = 1, nx - 1
      do j = 1, ny - 1
         vx(i, j) = (psi(i, j+1) - psi(i, j-1)) / (2 * delta)
         vy(i, j) = - (psi(i+1, j) - psi(i-1, j)) / (2 * delta)
      end do
   end do

   ! Zastawka

   do i = i1, i2
      do j = 0, j1
         vx(i ,j) = 0
         vy(i, j) = 0
      end do
   end do

   ! Brzegi góra dół

   do i = 1, nx - 1
      vx(i, 0) = 0
      vy(i, ny) = 0
   end do

   ! Prędkości sąsiadów na brzegach lewo prawo

   do j = 0, ny
      vx(0, j) = vx(1, j)
      vx(nx, j) = vx(nx -1, j)
   end do

   !
   ! vmax i dt
   !

   vmax = -1

   do i = 1, nx - 1
      do j = 1, ny - 1
         vtemp = sqrt(vx(i,j) **2 + vy(i,j) ** 2)
         if (vtemp > vmax) then
            vmax = vtemp
         end if
      end do
   end do

   dt = delta / (4 * vmax)

   print *, "Maximum v: ", vmax, " dt: ", dt

   !
   !  Warunek początkowy u od t=0
   !

   do i = 0, nx
      do j = 0, ny
         unow(i, j) = 1 / (2 * PI * omega ** 2) &
            * exp( -1 * ((i * delta - xa) **2 + (j * delta - ya) ** 2) / (2 * omega ** 2))
      end do
   end do

   !
   !  Zapisanie pola
   !

   open(1, file = './output/vy.dat')
   do i = 0, nx
      do j = 0, ny
         write(1,*) i * delta, j * delta, vy(i, j)
      end do
      write(1,*) ' '
   end do

   open(1, file = './output/vx.dat')
   do i = 0, nx
      do j = 0, ny
         write(1,*) i * delta, j * delta, vx(i, j)
      end do
      write(1,*) ' '
   end do

   close(1)

   !
   !  Pętla główna
   !

   do iteracja = 1, tmax

      ! Iteracja Picarda
      unext = unow
      upreviter = unow

      do k = 1, 20
         do i = 0, nx
            do j = 1, ny - 1
               ! Jeżeli zastawka, to ignoruj
               if (.not. (i >= i1 .and. i <= i2 .and. j <= j1)) then
                  if (i == 0) then
                     unext(i,j) = unow(i,j)
                  elseif (i == nx) then
                     unext(i,j) = unow(i,j)
                  else
                     firstterm = ((unow(i + 1, j) - unow(i - 1, j)) / (2 * delta)) + &
                      (upreviter(i+1, j) - upreviter(i - 1, j) / (2 * delta))

                     secondterm = ((unow(i, j +1) - unow(i, j-1)) / (2 * delta)) + &
                     ((upreviter(i, j + 1) - upreviter(i, j - 1)) / (2 * delta))
                     
                     unext(i,j) = unow(i,j) - 0.5 * dt * vx(i,j) * firstterm - 0.5 * dt * vy(i,j) * secondterm
                  end if

               end if
            end do
         end do
         upreviter = unext
      end do

      unow = unext

     

      write(trimnumber, '(I2)') iteracja

      write(fnamefull, '(A,A)') "./animout/zad5_it=", trim(adjustl(trim(trimnumber))) // ".txt"
      open(1, file = fnamefull)
      do i = 0, nx
         do j = 0, ny
            write(1,*) i * delta, j * delta, unow(i, j)
         end do
         write(1,*) ' '
      end do

      ! c oraz x sr

      ctsum = 0

      do i = 0, nx
         do j = 0, ny
            ctsum = ctsum + unow(i,j) * delta * delta
         end do
      end do

      ct(iteracja - 1) = ctsum

      

   end do


   open(1, file = './output/ct.dat')
   do i=1,tmax
      write(1,*) i, ct(i)
   end do

   close(1)

end program dyfuzjaprog
