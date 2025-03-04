C     Este progrma calcula la Integral de Coulomb para un QD eje-sim‚trico
C     El c lculo solo usa el estado fundamental para electr¢n y hueco (l=0, l: momentum angular)
C     Se usan las funciones ELLIPTICAS COMPLETAS DE PRIMERA CLASE
C     De ese modo se hace la integral angular para phi_e y phi_h entre 0 y 2 pi.

      implicit double precision (A-H,O-Z)
      parameter(np=19783)
      parameter(nw=33)

      dimension A_e(np,4),A_h(np,4)
      dimension A_ee(np,4),A_hh(np,4)
      dimension AMatrix_e(np,nw),AMatrix_h(np,nw)

      OPEN(1,file='xe_3D_full_out.txt',status='unknown')
      OPEN(2,file='xh_3D_full_out.txt',status='unknown')
      OPEN(3,file='exciton_3d_full.dat',status='unknown')

      OPEN(10,file='xe_2_f.txt',status='unknown')
      OPEN(11,file='xh_2_f.txt',status='unknown')

      pi =4.0d0*datan(1.0d0)
      !dro_e = 0.05d-9
      !dz_e  = 0.05d-9
      !dro_h = 0.05d-9
      !dz_h  = 0.05d-9
      dx_e  = 0.4d-9
      dy_e  = 0.4d-9
      dz_e  = 0.4d-9
      dx_h  = 0.4d-9
      dy_h  = 0.4d-9
      dz_h  = 0.4d-9


C     Leyendo la matrix de electrones y huecos para diferentes radios R_2 (desde 0 a 20 nm en pasos de 0.25, o sea 81 valores
      do 500 i=1,np
      read(1,*)(AMatrix_e(i,j),j=1,nw)
      read(2,*)(AMatrix_h(i,j),j=1,nw)
  500 continue


C     en el siguiente loop se corren los 81 valores de R_2
      R_2 = 0.1d0
      do 1000 k=1,nw-3
C     lectura de posiciones y funciones de onda del estado fundamental de electr¢n
C     tambi‚n se identifica la funci¢n de onda m xima de la lista
      elec_max = 0.0d0
      do 1 i=1,np
      A_ee(i,1) = AMatrix_e(i,1)
      A_ee(i,2) = AMatrix_e(i,2)
      A_ee(i,3) = AMatrix_e(i,3)
      A_ee(i,4) = AMatrix_e(i,3+k)
      elec_max = max(elec_max,A_ee(i,4))
  1   continue
c      write(*,*)elec_max

C     selecci¢n de las funciones de onda que son mayores o iguales que el 1%
C     del valor m ximo
      nt=1
      do 100 i=1,np
      if(A_ee(i,4).ge.0.01d0*elec_max)then
      A_e(nt,1)=A_ee(i,1)
      A_e(nt,2)=A_ee(i,2)
      A_e(nt,3)=A_ee(i,3)
      A_e(nt,4)=A_ee(i,4)/elec_max
      nt=nt+1
      else
      endif
  100 continue
      n1 = nt-1
c      write(*,*)n1

      do 30 i=1,n1
      write(10,*)A_e(i,1),A_e(i,2),A_e(i,3),A_e(i,4)
  30   continue

C     lectura de posiciones y funciones de onda del estado fundamental de hueco
C     tambi‚n se identifica la funci¢n de onda m xima de la lista
      hole_max = 0.0d0
      do 10 i=1,np
      A_hh(i,1) = AMatrix_h(i,1)
      A_hh(i,2) = AMatrix_h(i,2)      
      A_hh(i,3) = AMatrix_h(i,3)
      A_hh(i,4) = AMatrix_h(i,3+k)
      hole_max = max(hole_max,A_hh(i,4))
  10   continue
c      write(*,*)hole_max

C     selecci¢n de las funciones de onda que son mayores o iguales que el 1%
C     del valor m ximo
      nq=1
      do 200 i=1,np
      if(A_hh(i,4).ge.0.01d0*hole_max)then
      A_h(nq,1)=A_hh(i,1)
      A_h(nq,2)=A_hh(i,2)
      A_h(nq,3)=A_hh(i,3)
      A_h(nq,4)=A_hh(i,4)/hole_max
      nq=nq+1
      else
      endif
  200 continue
      n2 = nq-1
c      write(*,*)n2
c      read(*,*)absbs

      do 40 i=1,n2
      write(11,*)A_h(i,1),A_h(i,2),A_h(i,3),A_h(i,4)
  40   continue


C     n1: n£mero de funciones de onda de electr¢n que son mayores
C     o iguales que el 1% del valor m ximo
C     n2: n£mero de funciones de onda de hueco que son mayores
C     o iguales que el 1% del valor m ximo


C     normalizar las funciones de onda    Int. Psi^2*2*pi*ro*dro*dz
      suma_e = 0.0d0
      do 2 i=1,n1
      !ro_e = A_e(i,1)*1.0d-9
      !dVe  = 2.0d0*pi*ro_e*dro_e*dz_e
      dVe  = dx_e*dy_e*dz_e
      psi_e= A_e(i,4)
      suma_e = suma_e+psi_e**2*dVe
  2   continue
      xnorm_e = suma_e


      suma_h = 0.0d0
      do 3 i=1,n2
      !ro_h = A_h(i,1)*1.0d-9
      !dVh  = 2.0d0*pi*ro_h*dro_h*dz_h
      dVh  = dx_h*dy_h*dz_h
      psi_h= A_h(i,4)
      suma_h = suma_h+psi_h**2*dVh
  3   continue
      xnorm_h = suma_h

C     Calculemos la interacci¢n de Coulomb
C      go to 20
      suma_C = 0.0d0
      do 4 i=1,n1
      do 5 j=1,n2
      x_e = A_e(i,1)*1.0d-9
      x_h = A_h(j,1)*1.0d-9
      y_e = A_e(i,2)*1.0d-9
      y_h = A_h(j,2)*1.0d-9
      z_e = A_e(i,3)*1.0d-9
      z_h = A_h(j,3)*1.0d-9
      
      r = dsqrt((x_e-x_h)**2+(y_e-y_h)**2+(z_e-z_h)**2)
      if(r.eq.0.0d0)go to 5
      psi_e = A_e(i,4)/dsqrt(xnorm_e)
      psi_h = A_h(j,4)/dsqrt(xnorm_h)
      
c      dVe = ro_e*2.0d0*pi*dro_e*dz_e
c      dVh = ro_h*2.0d0*pi*dro_h*dz_h
c      suma_C = suma_C+psi_e**2*psi_h**2*dVe*dVh/r
C     las tres lineas anterior las vamos a cambiar por las siguientes lineas
C     donde la integral angular en phi_e y phi_h  de  (1/r) entre 0 y 2 pi queda expresada
C     en t‚rminos de funciones ELIPTICAS
      !dVe = ro_e*dro_e*dz_e        !notese que he quitado el factor 2*pi
      !dVh = ro_h*dro_h*dz_h        !notese que he quitado el factor 2*pi
!C     SUBROUTINE COMELP(XK,CK,CE)  Calcula la funci¢n Elptica
      !rp = 4.0d0*ro_e*ro_h/r
      !XK = rp/(1.0d0+rp)
      !call COMELP(XK,CK,CE)
      !xEliptica = CK
      !xIntegral_Angular = 8.0d0*pi/r*xEliptica/dsqrt(1.0d0+rp)
      !suma_C = suma_C+psi_e**2*psi_h**2*dVe*dVh !*xIntegral_Angular
      suma_C = suma_C + psi_e**2 * psi_h**2 * dVe * dVh / r  ! Agregar 1/r
      
  5   continue
  4   continue
      xcoulomb = suma_C
  20  continue



! C     Calculo de la posici¢n z_e del electr¢n, z_h del hueco y la integral de overlap
!       suma_ze = 0.0d0
!       suma_zh = 0.0d0
!       suma_overlap = 0.0d0
!       do 41 j=1,np
!       ro_e = A_ee(j,1)*1.0d-9
!       ro_h = A_hh(j,1)*1.0d-9
!       z_e = A_ee(j,2)*1.0d-9
!       z_h = A_hh(j,2)*1.0d-9

!       psi_e = A_ee(j,3)/dsqrt(xnorm_e)
!       psi_h = A_hh(j,3)/dsqrt(xnorm_h)

!       dVe = ro_e*2.0d0*pi*dro_e*dz_e
!       dVh = ro_h*2.0d0*pi*dro_h*dz_h
!       suma_ze = suma_ze+psi_e**2*z_e*dVe
!       suma_zh = suma_zh+psi_h**2*z_h*dVh
!       suma_overlap = suma_overlap+psi_e*psi_h*dVe
!   41   continue
!       xIntegral_overlap=suma_overlap**2

C     constantes y valores usados en los c lculos
      h = 6.62606876d-34             ! Planck's constant [J*s]
      hbar = h/(2.0d0*pi)            ! Planck's constant reduced [J*s]
      epsilon0 = 8.854187817d-12     ! Vacuum permittivity [F/m] = [A*s/(V*m)]
      xme0 = 9.10938188d-31          ! Electron mass [kg]
      c = 299792458                  ! speed of light in m/s
      e = 1.60217662d-19             ! elementary charge in coulombs
      xJeV = 1.0d0/1.60217646d-19    ! Joule in eV

! C     GaAs related constants,
!       xme_GaAs  = 0.067d0*xme0        ! GaAs effective electron mass [kg]
!       xmhh_GaAs = 0.51d0*xme0        ! GaAs effective heavy hole mass [kg]
      epsilon_GaAs = 10.2d0*epsilon0 ! GaAs permittivity

!       xm_Ex = xme_GaAs*xmhh_GaAs/(xme_GaAs+xmhh_GaAs)          ! Exciton effective mass
!       r_Ex_GaAs = 4.0d0*pi*epsilon_GaAs*hbar**2/(xm_Ex*e**2)   ! GaAs exciton Bohr radius in [m]
!       E_Ry_GaAs = xm_Ex*e**4/(2.0d0*(2.0d0*epsilon_GaAs*h)**2) ! Rydberg constant in GaAs bulk in [J]
C     *******************************************
      constante = e**2/(4.0d0*pi*epsilon_GaAs)*xJeV*1.0d3

      E_eh = constante*xcoulomb
      ! E_eh = E_eh/2.0d0         ! ESTE PASO DE DIVIDIR ENTRE DOS SE DEBE A QUE HAY QUE TOMAR SOLO LA MITAD DEL SEMIPLANO

      write(3,*)R_2,E_eh!,suma_ze,suma_zh,xIntegral_overlap
      write(*,*)R_2,E_eh!,suma_ze,suma_zh,xIntegral_overlap
      R_2 = R_2+0.1d0

 1000 continue
      WRITE(*,*)'DEME CUALQUIER COSA PARA TERMINAR EL PROGRAMA Coulomb2'
      read(*,*)SSSSS
      stop
      end


! C       ==================================================
!         SUBROUTINE COMELP(XK,CK,CE)
! C       COMENTARIO: lo que en este programa se evalua poniendo la variable como x
! C       en el mathematica hay que poner x^2. Por esa raz¢n se redefine la variabble
! C       en la usando la raiz cuadrada. Ver la l¡nea insertada despu‚s del
! C       IMPLICIT DOUBLE PRECISION

! C
! C
! C       ==================================================
! C       Purpose: Compute complete elliptic integrals K(k)
! C                and E(k)
! C       Input  : K  --- Modulus k ( 0 ó k ó 1 )
! C       Output : CK --- K(k)
! C                CE --- E(k)
! C       ==================================================
! C
!         IMPLICIT DOUBLE PRECISION (A-H,O-Z)
!         HK = dsqrt(XK)

!         PK=1.0D0-HK*HK
!         IF (HK.EQ.1.0) THEN
!            CK=1.0D+300
!            CE=1.0D0
!         ELSE
!            AK=(((.01451196212D0*PK+.03742563713D0)*PK
!      &        +.03590092383D0)*PK+.09666344259D0)*PK+
!      &        1.38629436112D0
!            BK=(((.00441787012D0*PK+.03328355346D0)*PK+
!      &        .06880248576D0)*PK+.12498593597D0)*PK+.5D0
!            CK=AK-BK*DLOG(PK)
!            AE=(((.01736506451D0*PK+.04757383546D0)*PK+
!      &        .0626060122D0)*PK+.44325141463D0)*PK+1.0D0
!            BE=(((.00526449639D0*PK+.04069697526D0)*PK+
!      &        .09200180037D0)*PK+.2499836831D0)*PK
!            CE=AE-BE*DLOG(PK)
!         ENDIF
!         RETURN
!         END


