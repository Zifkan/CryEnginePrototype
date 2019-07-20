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
   <Fragment BlendOutDuration="0.2" Tags="" FragTags="RightSide">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="barb_1hnd_rct_left_hit_front_lf_to_front_lf_03"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="" FragTags="LeftSide">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="barb_1hnd_rct_right_hit_front_lf_to_front_rf_01"/>
    </AnimLayer>
   </Fragment>
  </HitReaction>
  <Death>
   <Fragment BlendOutDuration="0.2" Tags="" FragTags="Dead">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="1.9" Duration="0"/>
     <Animation name="bar_archr_fire_M_01"/>
     <Blend ExitTime="1" StartTime="0" Duration="0.2"/>
     <Animation name=""/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="" FragTags="Dying">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="Bar_Shld_blk_front_lf_to_front_rf_stat_01"/>
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
 <FragmentBlendList>
  <Blend from="Attack" to="Attack">
   <Variant from="Simple" to="Force">
    <Fragment BlendOutDuration="0.2" selectTime="0" enterTime="0">
     <AnimLayer>
      <Blend ExitTime="-1" StartTime="0" Duration="0.5599997" terminal="1"/>
     </AnimLayer>
    </Fragment>
   </Variant>
  </Blend>
 </FragmentBlendList>
</AnimDB>
