(* program designed to study
 Dawkins' biomorphs.
 See: The Blind Watch Maker *)

{$N+} (* numeric processing *)
{$R+} (* range checking *)
{$S+} (* stack checking *)

program dawkins8;

uses genecode,crt,graph;

const
   rad_factor =  57.2957795;

var gene:chromosome; (* in genecode unit
