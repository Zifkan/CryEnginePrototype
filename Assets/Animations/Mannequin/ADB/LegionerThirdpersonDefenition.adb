<AnimDB FragDef="Animations/Mannequin/ADB/LegionPlayerFragmentIds.xml" TagDef="Animations/Mannequin/ADB/legionertags.xml">
 <FragmentList>
  <Idle>
   <Fragment BlendOutDuration="0.2" Tags="">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="romn_ready_idle_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
  </Idle>
  <Walk>
   <Fragment BlendOutDuration="0.2" Tags="">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="romn_com_move_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
  </Walk>
  <Attack>
   <Fragment BlendOutDuration="0.2" Tags="Simple">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="comm_com_atk_combo_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Simple">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="comm_com_atk_combo_02"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Simple">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="romn_atk_0_fwd_stat_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Simple">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="barb_1hnd_atk_strg_lft_up_Lf_to_Rf_150_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Force">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="barb_1hnd_atk_qk_lft_up_from_chrg_Rf_250_02"/>
    </AnimLayer>
   </Fragment>
  </Attack>
  <HitReaction>
   <Fragment BlendOutDuration="0.2" Tags="" FragTags="LeftSide">
    <AnimLayer />
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="" FragTags="RightSide">
    <AnimLayer />
   </Fragment>
  </HitReaction>
  <Death>
   <Fragment BlendOutDuration="0.2" Tags="">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="barb_1hnd_death_from_front_lf"/>
    </AnimLayer>
   </Fragment>
  </Death>
  <Block>
   <Fragment BlendOutDuration="0.2" Tags="">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="evr_stand_com_shld_block_idle_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
  </Block>
 </FragmentList>
</AnimDB>
