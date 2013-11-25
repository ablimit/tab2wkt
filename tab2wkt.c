#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "mitab_capi.h"

#ifndef NULL
#  define NULL 0
#endif

/************************************************************************/
/*                              DumpFile                                */
/*                 dumps a tab file to a wkt file.                      */
/************************************************************************/

void DumpFile( const char * filename )

{
  mitab_handle	dataset;
  int			feature_id, num_fields;

  dataset = mitab_c_open( filename );

  if( dataset == NULL )
  {
    printf( "mitab_c_open(%s) failed.\n%s\n",
        filename, mitab_c_getlasterrormsg() );
    exit( 1 );
  }

  num_fields = mitab_c_get_field_count(dataset);

  for( feature_id = mitab_c_next_feature_id(dataset,-1);
      feature_id != -1;
      feature_id = mitab_c_next_feature_id(dataset,feature_id) )
  {
    mitab_feature	feature;
    int feature_type, num_parts, partno, pointno, fieldno;

    /* -------------------------------------------------------------------- */
    /*      Read next feature object                                        */
    /* -------------------------------------------------------------------- */
    feature = mitab_c_read_feature( dataset, feature_id );
    if( feature == NULL )
    {
      fprintf(stderr, "Failed to read feature %d.\n%s\n",
          feature_id, mitab_c_getlasterrormsg() );
      exit( 1 );
    }

    feature_type = mitab_c_get_type(feature);
    num_parts = mitab_c_get_parts(feature);
    if (num_parts > 1 )
    {
      fprintf(stderr, "ERROR: Can NOT handle multipart spatial objects. part = %d.", num_parts );
      exit( 1 );
    }

   // printf( "Read feature %d: type=%d, num_parts=%d.\n", feature_id, feature_type, num_parts  );

    /* -------------------------------------------------------------------- */
    /*      Dump the feature attributes...                                  */
    /* -------------------------------------------------------------------- */
    /*
    for(fieldno = 0; fieldno < num_fields; fieldno++)
    {
      printf("  %s = %s\n", 
          mitab_c_get_field_name(dataset, fieldno),
          mitab_c_get_field_as_string(feature, fieldno) );
    }*/

    /* -------------------------------------------------------------------- */
    /*      ... and coordinates.                                            */
    /*      In real applications, we would probably want to handle each     */
    /*      object type differently but we won't do it here.                */
    /* -------------------------------------------------------------------- */
    for(partno = 0; partno < num_parts; partno++)
    {
      int num_points = mitab_c_get_vertex_count(feature, partno);

      /* 
       * if (num_parts > 1)
        printf(" Part no %d:\n", partno);
      */
      printf("%d\tPOLYGON((", feature_id);
      for(pointno = 0; pointno < num_points - 1 ; pointno++)
      {
        double dX, dY;
        dX = mitab_c_get_vertex_x(feature, partno, pointno);
        dY = mitab_c_get_vertex_y(feature, partno, pointno);

        printf("%.16g %.16g, ", dX, dY);
      }
      // last point 
      printf("%.16g %.16g))\n",mitab_c_get_vertex_x(feature, partno, 0), mitab_c_get_vertex_y(feature, partno, 0));
    }
    mitab_c_destroy_feature( feature );
  }

  mitab_c_close( dataset );

  if( mitab_c_getlasterrormsg() != NULL 
      && strlen(mitab_c_getlasterrormsg()) > 0 )
    fprintf( stderr, "Last Error: %s\n", mitab_c_getlasterrormsg() );
}
/************************************************************************/
/*                                main()                                */
/************************************************************************/

int main( int argc , char ** argv )

{
  if( argc < 2 )
  {
    printf( "Usage: tab2wkt filename \n" );
    exit( 1 );
  }

  DumpFile( argv[1] );

  exit( 0 );
}

